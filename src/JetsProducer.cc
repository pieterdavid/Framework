#include <cp3_llbb/Framework/interface/JetsProducer.h>
#include <cp3_llbb/Framework/interface/Tools.h>
#include "DataFormats/Math/interface/deltaR.h"

void JetsProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    edm::Handle<std::vector<pat::Jet>> jets;
    event.getByToken(m_jets_token, jets);

    edm::Handle<double> rho_handle;
    event.getByToken(m_rho_token, rho_handle);

    for (const auto &jet: *jets) {
        if (! pass_cut(jet))
            continue;
        fill_candidate(jet, jet.genJet());

        jecFactor.push_back(jet.jecFactor(0));
        area.push_back(jet.jetArea());
        partonFlavor.push_back(jet.partonFlavour());
        hadronFlavor.push_back(jet.hadronFlavour());
        // Variables needed for 74X b-jet energy regression as of January 26th 2016
        neutralHadronEnergyFraction.push_back(jet.neutralHadronEnergyFraction());
        neutralEmEnergyFraction.push_back(jet.neutralEmEnergyFraction());
        vtx3DVal.push_back(jet.userFloat("vtx3DVal"));
        vtx3DSig.push_back(jet.userFloat("vtx3DSig"));
        float vtxPx = jet.userFloat("vtxPx");
        float vtxPy = jet.userFloat("vtxPy");
        float vtxPt_ = std::sqrt(std::pow(vtxPx,2) + std::pow(vtxPy,2));
        vtxPt.push_back(vtxPt_);
        vtxNtracks.push_back(jet.userFloat("vtxNtracks"));
        // some loops needed to dig some information within the jet itself
        std::vector<std::pair<float, int>> softLeptInJet; // pt, index
        softLeptInJet.clear();
        float leadTrackPt_ = 0.;
        for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); idaughter++)
        {
            const reco::Candidate * daughter = jet.daughter(idaughter);
            if (std::abs(daughter->pdgId()) == 11 || std::abs(daughter->pdgId()) == 13)
                softLeptInJet.push_back( std::make_pair(daughter->pt(), idaughter) );
            if (daughter->charge() != 0)
            {
                float leadTrackPt_tmp = daughter->pt();
                if(leadTrackPt_tmp > leadTrackPt_)
                    leadTrackPt_ = leadTrackPt_tmp;
            }
        }
        leadTrackPt.push_back(leadTrackPt_);
        float leptonPtRel_ = -1.;
        float leptonPt_ = -1.;
        float leptonDeltaR_ = -1.;
        int softLeptIdx_ = -1;
        if (softLeptInJet.size() > 0)
        {
            std::sort(softLeptInJet.begin(), softLeptInJet.end());
            softLeptIdx_ = softLeptInJet.back().second;
            const reco::Candidate *daughter = jet.daughter(softLeptIdx_);
            leptonPt_ = daughter->pt();
            leptonPtRel_ = daughter->pt() / jet.pt();
            leptonDeltaR_ = deltaR(*daughter, jet);
        }
        leptonPtRel.push_back(leptonPtRel_);
        leptonPt.push_back(leptonPt_);
        leptonDeltaR.push_back(leptonDeltaR_);
        chargedMultiplicity.push_back(jet.chargedMultiplicity());
        // The regression output itself
        if (computeRegression)
        {
            Jet_pt = jet.pt();
            Jet_corr = jet.jecFactor("Uncorrected");
            rho = *rho_handle;
            Jet_eta = jet.eta();
            Jet_mt = jet.mt();
            Jet_leadTrackPt = leadTrackPt_;
            Jet_leptonPtRel = leptonPtRel_;
            Jet_leptonPt = leptonPt_;
            Jet_leptonDeltaR = leptonDeltaR_;
            Jet_neHEF = jet.neutralHadronEnergyFraction();
            Jet_neEmEF = jet.neutralEmEnergyFraction();
            Jet_chMult = jet.chargedMultiplicity();
            Jet_vtxPt = vtxPt_;
            Jet_vtxMass = jet.userFloat("vtxMass");
            Jet_vtx3dL = jet.userFloat("vtx3DVal");
            Jet_vtxNtrk = jet.userFloat("vtxNtracks");
            Jet_vtx3deL = jet.userFloat("vtx3DSig");
            regPt.push_back((bjetRegressionReader->EvaluateRegression("BDTG method"))[0]);
        } else {
            regPt.push_back(jet.pt());
        }

        passLooseID.push_back(Tools::Jets::passLooseId(jet));
        passTightID.push_back(Tools::Jets::passTightId(jet));
        passTightLeptonVetoID.push_back(Tools::Jets::passTightLeptonVetoId(jet));

        if (jet.hasUserFloat("pileupJetId:fullDiscriminant"))
            puJetID.push_back(jet.userFloat("pileupJetId:fullDiscriminant"));

        vtxMass.push_back(jet.userFloat("vtxMass"));

        for (auto& it: m_btag_discriminators) {

            float btag_discriminator = jet.bDiscriminator(it.first);
            // Protect against NaN discriminant
            if (std::isnan(btag_discriminator))
                btag_discriminator = -10;

            it.second->push_back(btag_discriminator);

            Algorithm algo = string_to_algorithm(it.first);
            if (algo != Algorithm::UNKNOWN && BTaggingScaleFactors::has_scale_factors(algo)) {
                BTaggingScaleFactors::store_scale_factors(algo, get_flavor(jet.hadronFlavour()), {static_cast<float>(std::abs(jet.eta())), static_cast<float>(jet.pt()), btag_discriminator}, event.isRealData());
            }
        }
    }
}
