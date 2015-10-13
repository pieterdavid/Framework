#include <cp3_llbb/Framework/interface/FatJetsProducer.h>
#include <DataFormats/BTauReco/interface/CATopJetTagInfo.h>


void FatJetsProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    edm::Handle<std::vector<pat::Jet>> jets;
    event.getByToken(m_jets_token, jets);

    for (const auto& jet: *jets) {
        if (! pass_cut(jet))
            continue;

        fill_candidate(jet, jet.genJet());

        jecFactor.push_back(jet.jecFactor(0));
        area.push_back(jet.jetArea());
        partonFlavor.push_back(jet.partonFlavour());
        hadronFlavor.push_back(jet.hadronFlavour());

        tau1.push_back(jet.userFloat(Njettinesstau1));
        tau2.push_back(jet.userFloat(Njettinesstau2));
        tau3.push_back(jet.userFloat(Njettinesstau3));

        softdrop_mass.push_back(jet.userFloat("ak8PFJetsCHSSoftDropMass"));
        trimmed_mass.push_back(jet.userFloat("ak8PFJetsCHSTrimmedMass"));
        pruned_mass.push_back(jet.userFloat("ak8PFJetsCHSPrunedMass"));
        filtered_mass.push_back(jet.userFloat("ak8PFJetsCHSFilteredMass"));

        reco::CATopJetTagInfo const * tagInfo =  dynamic_cast<reco::CATopJetTagInfo const *>(jet.tagInfo("caTop"));
        if (tagInfo) {
            has_toptag_info.push_back(true);
            toptag_min_mass.push_back(tagInfo->properties().minMass);
            toptag_top_mass.push_back(tagInfo->properties().topMass);
            toptag_w_mass.push_back(tagInfo->properties().wMass);
            toptag_n_subjets.push_back(tagInfo->properties().nSubJets);
        } else {
            has_toptag_info.push_back(false);
            toptag_min_mass.push_back(0);
            toptag_top_mass.push_back(0);
            toptag_w_mass.push_back(0);
            toptag_n_subjets.push_back(0);
        }

        // Subjets
        // 1) SoftDrop
        const auto& wSubjets = jet.subjets("SoftDrop");
        std::map<std::string, std::vector<float>> subjets_btag_discriminators;
        std::vector<LorentzVector> subjets_p4;
        for (const auto& iw: wSubjets) {
            subjets_p4.push_back(LorentzVector(iw->pt(), iw->eta(), iw->phi(), iw->energy()));
            for (const auto& subjet_btag: m_subjets_btag_discriminators) {
                subjets_btag_discriminators[subjet_btag].push_back(iw->bDiscriminator(subjet_btag));
            }
        }

        softdrop_subjets_p4.push_back(subjets_p4);
        for (const auto& it: subjets_btag_discriminators) {
            m_softdrop_btag_discriminators_branches[it.first]->push_back(it.second);
        }

        // 2) Top tagger
        const auto& wTopjets = jet.subjets("CMSTopTag");
        subjets_p4.clear();
        subjets_btag_discriminators.clear();
        for (const auto& it: wTopjets) {
            subjets_p4.push_back(LorentzVector(it->pt(), it->eta(), it->phi(), it->energy()));
            for (const auto& subjet_btag: m_subjets_btag_discriminators) {
                subjets_btag_discriminators[subjet_btag].push_back(it->bDiscriminator(subjet_btag));
            }
        }

        toptag_subjets_p4.push_back(subjets_p4);
        for (const auto& it: subjets_btag_discriminators) {
            m_toptag_btag_discriminators_branches[it.first]->push_back(it.second);
        }

        for (auto& it: m_btag_discriminators) {

            float btag_discriminator = jet.bDiscriminator(it.first);
            it.second->push_back(btag_discriminator);

            Algorithm algo = string_to_algorithm(it.first);
            if (algo != Algorithm::UNKNOWN && BTaggingScaleFactors::has_scale_factors(algo)) {
                BTaggingScaleFactors::store_scale_factors(algo, get_flavor(jet.hadronFlavour()), {static_cast<float>(fabs(jet.eta())), static_cast<float>(jet.pt()), btag_discriminator});
            }
        }
    }
}
