
#include <cp3_llbb/Framework/interface/JetsProducer.h>

void JetsProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

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

        if (jet.hasUserFloat("pileupJetId:fullDiscriminant"))
            puJetID.push_back(jet.userFloat("pileupJetId:fullDiscriminant"));

        vtxMass.push_back(jet.userFloat("vtxMass"));

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
