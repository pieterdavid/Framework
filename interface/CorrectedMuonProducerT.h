#pragma once

/** \class CorrectedMuonProducerT
 *
 * Produce collection of corrected muons using either KaMuCa or Rochester corrections.
 *
 * \author Sébastien Brochet
 *
 */

#include "CommonTools/Utils/interface/PtComparator.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include <cp3_llbb/Framework/interface/rochcor2016.h>
#include <KaMuCa/Calibration/interface/KalmanMuonCalibrator.h>

#include <memory>

namespace cp3 {

    class KaMuCaCorrector {
        public:
            explicit KaMuCaCorrector(const edm::ParameterSet& cfg) {
                std::string tag = cfg.getParameter<std::string>("tag");
                corrector.reset(new KalmanMuonCalibrator(tag));
            }

            template<typename T>
            T correct(const edm::Event& event, const T& muon) {
                auto corrected_pt = corrector->getCorrectedPt(muon.pt(), muon.eta(), muon.phi(), muon.charge());
                if (! event.isRealData())
                    corrected_pt = corrector->smear(corrected_pt, muon.eta());

                double ratio = corrected_pt / muon.pt();

                T corrected_muon = muon;
                corrected_muon.setP4(corrected_muon.p4() * ratio);

                return corrected_muon;
            }

        private:
            std::unique_ptr<KalmanMuonCalibrator> corrector;
    };

    class RochesterCorrector {
        public:
            explicit RochesterCorrector(const edm::ParameterSet& cfg) {
                auto tag = cfg.getParameter<edm::FileInPath>("input");
                corrector.reset(new rochcor2016(tag.fullPath()));
            }

            template<typename T>
            T correct(const edm::Event& event, const T& muon) {

                float qter = 0;

                TLorentzVector muon_p4(muon.px(), muon.py(), muon.pz(), muon.energy());

                if (event.isRealData()) {
                    corrector->momcor_data(muon_p4, muon.charge(), 0, qter);
                } else {
                    int ntrk = 0;
                    if (!muon.innerTrack().isNull())
                        ntrk = muon.innerTrack()->hitPattern().trackerLayersWithMeasurement();

                    corrector->momcor_mc(muon_p4, muon.charge(), ntrk, qter);
                }

                T corrected_muon = muon;
                corrected_muon.setP4(math::XYZTLorentzVector(muon_p4.Px(), muon_p4.Py(), muon_p4.Pz(), muon_p4.E()));

                return corrected_muon;
            }

        private:
            std::unique_ptr<rochcor2016> corrector;
    };
}

template <typename T, class C>
class CorrectedMuonProducerT : public edm::stream::EDProducer<> {

    using MuonCollection = std::vector<T>;

    public:
    explicit CorrectedMuonProducerT(const edm::ParameterSet& cfg) {

        enabled = cfg.getParameter<bool>("enabled");
        muons_token = consumes<MuonCollection>(cfg.getParameter<edm::InputTag>("src"));

        if (enabled) {
            corrector.reset(new C(cfg));
        }

        produces<MuonCollection>();
        produces<edm::ValueMap<float>>("correctionFactors");
    }

    virtual void produce(edm::Event& event, const edm::EventSetup& setup) override {

        edm::Handle<MuonCollection> muons_collection;
        event.getByToken(muons_token, muons_collection);

        const MuonCollection& muons = *muons_collection;

        std::unique_ptr<MuonCollection> corrected_muons(new MuonCollection());
        std::vector<float> correction_factors;

        for (const auto& muon: muons) {
            if ((! enabled) || muon.pt() == 0) {
                corrected_muons->push_back(muon);
                correction_factors.push_back(1);
                continue;
            }

            // Correct muon
            T corrected_muon = corrector->template correct<T>(event, muon);

            double ratio = corrected_muon.pt() / muon.pt();

            corrected_muons->push_back(corrected_muon);
            correction_factors.push_back(ratio);
        }

        std::unique_ptr<edm::ValueMap<float>> correction_factors_map(new edm::ValueMap<float>());
        edm::ValueMap<float>::Filler filler(*correction_factors_map);
        filler.insert(muons_collection, correction_factors.begin(), correction_factors.end());
        filler.fill();

        std::sort(corrected_muons->begin(), corrected_muons->end(), muon_pt_comparator);

        event.put(std::move(corrected_muons));
        event.put(std::move(correction_factors_map), "correctionFactors");
    }

    private:
    edm::EDGetTokenT<MuonCollection> muons_token;
    bool enabled;

    std::unique_ptr<C> corrector;

    GreaterByPt<T> muon_pt_comparator;
};
