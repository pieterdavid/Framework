#ifndef PhysicsTools_PatUtils_ShiftedJetProducerWithSourcesT_h
#define PhysicsTools_PatUtils_ShiftedJetProducerWithSourcesT_h

#include "CommonTools/Utils/interface/PtComparator.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "FWCore/Framework/interface/ESHandle.h"

template <typename T>
class ShiftedJetProducerWithSourcesT : public edm::stream::EDProducer<> {
    typedef std::vector<T> JetCollection;

    public:

    explicit ShiftedJetProducerWithSourcesT(const edm::ParameterSet& cfg) :
        m_enabled(cfg.getParameter<bool>("enabled")),
        m_debug(cfg.getUntrackedParameter<bool>("debug", false)),
        m_split_by_sources(cfg.getParameter<bool>("splitBySources")) {

            m_jets_token = consumes<JetCollection>(cfg.getParameter<edm::InputTag>("src"));

            if (m_enabled) {
                shiftBy_ = cfg.getParameter<double>("shiftBy");

                m_tag = "Total";
                if (cfg.exists("sourceTag")) {
                    m_tag = cfg.getParameter<std::string>("sourceTag");
                }

                if (cfg.exists("sources")) {
                    const std::string sourcesFile = cfg.getParameter<edm::FileInPath>("sources").fullPath();
                    JetCorrectorParameters parameters(sourcesFile, m_tag);
                    std::unique_ptr<JetCorrectionUncertainty> uncertainty(new JetCorrectionUncertainty(parameters));
                    jecUncProvider = std::move(uncertainty);

                    if (m_split_by_sources) {
                        for (const auto& source: m_jec_sources) {
                            JetCorrectorParameters source_parameters(sourcesFile, source);
                            std::unique_ptr<JetCorrectionUncertainty> source_uncertainty(new JetCorrectionUncertainty(source_parameters));
                            jecSourceUncProviders.emplace(source, std::move(source_uncertainty));
                        }
                    }
                } else if (m_tag != "Total") {
                    throw edm::Exception(edm::errors::LogicError, "You must provide a 'sources' text file for any tag different than 'Total' (only total uncertainty is stored in the Global Tag)");
                }
            }

            produces<JetCollection>();
            if (m_split_by_sources) {
                for (const auto& source: m_jec_sources)
                    produces<JetCollection>(source);
            }
        }

    private:

    void beginRun(edm::Run const &, edm::EventSetup const &setup) {
        // Construct an object to obtain JEC uncertainty [1]
        //[1] https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections?rev=137#JetCorUncertainties
        if (! jecUncProvider) {
            edm::ESHandle<JetCorrectorParametersCollection> jecParametersCollection;
            setup.get<JetCorrectionsRecord>().get("AK4PFchs", jecParametersCollection); 

            JetCorrectorParameters const &jecParameters = (*jecParametersCollection)["Uncertainty"];
            jecUncProvider.reset(new JetCorrectionUncertainty(jecParameters));
        }
    }

    void produce(edm::Event& event, const edm::EventSetup& es) {

        edm::Handle<JetCollection> jets;
        event.getByToken(m_jets_token, jets);

        std::unique_ptr<JetCollection> shiftedJets(new JetCollection());
        std::map<std::string, std::unique_ptr<JetCollection>> sourceShiftedJets;
        if (jecSourceUncProviders.size() > 0) {
            for (const auto& source: m_jec_sources) {
                sourceShiftedJets.emplace(source, std::move(std::unique_ptr<JetCollection>(new JetCollection())));
            }
        }

        for (const auto& jet: *jets) {

            if (! m_enabled) {
                shiftedJets->emplace_back(jet);
                continue;
            }

            jecUncProvider->setJetEta(jet.eta());
            jecUncProvider->setJetPt(jet.pt());

            double const combinedJecUncertainty = shiftBy_ * std::abs(jecUncProvider->getUncertainty(true));

            T shiftedJet(jet);
            shiftedJet.scaleEnergy(1 + combinedJecUncertainty);

            if (jecSourceUncProviders.size() > 0) {
                for (const auto& it: jecSourceUncProviders) {
                    if (it.first == "Total")
                        continue;

                    it.second->setJetEta(jet.eta());
                    it.second->setJetPt(jet.pt());

                    float uncertainty = it.second->getUncertainty(true);

                    T partialShiftedJet(jet);
                    partialShiftedJet.scaleEnergy(1 + shiftBy_ * uncertainty);
                    sourceShiftedJets[it.first]->emplace_back(partialShiftedJet);
                }
            }

            shiftedJets->emplace_back(shiftedJet);
        }

        // Sort jets by pt
        std::sort(shiftedJets->begin(), shiftedJets->end(), jetPtComparator);
        event.put(std::move(shiftedJets));

        for (auto& it: sourceShiftedJets) {
            std::sort(it.second->begin(), it.second->end(), jetPtComparator);
            event.put(std::move(it.second), it.first);
        }
    }

    bool m_enabled;
    bool m_debug;
    std::string m_tag;
    bool m_split_by_sources;

    edm::EDGetTokenT<JetCollection> m_jets_token;
    double shiftBy_; // set to +1.0/-1.0 for up/down variation of energy scale

    std::unique_ptr<JetCorrectionUncertainty> jecUncProvider;
    std::map<std::string, std::unique_ptr<JetCorrectionUncertainty>> jecSourceUncProviders;

    GreaterByPt<T> jetPtComparator;

    // Updated list from https://hypernews.cern.ch/HyperNews/CMS/get/jes/648/1/1/1.html
    std::vector<std::string> m_jec_sources = {
        "AbsoluteFlavMap",
        "AbsoluteMPFBias",
        "AbsoluteScale",
        "AbsoluteStat",
        "FlavorQCD",
        "Fragmentation",
        "PileUpDataMC",
        "PileUpPtBB",
        "PileUpPtEC1",
        "PileUpPtEC2",
        "PileUpPtHF",
        "PileUpPtRef",
        "RelativeBal",
        "RelativeFSR",
        "RelativeJEREC1",
        "RelativeJEREC2",
        "RelativeJERHF",
        "RelativePtBB",
        "RelativePtEC1",
        "RelativePtEC2",
        "RelativePtHF",
        "RelativeStatEC",
        "RelativeStatFSR",
        "RelativeStatHF",
        "SinglePionECAL",
        "SinglePionHCAL",
        "TimePtEta"
    };
};

#endif



