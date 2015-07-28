
#include <cp3_llbb/Framework/interface/GenParticlesProducer.h>

void GenParticlesProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    edm::Handle<std::vector<pat::PackedGenParticle>> packed_gen_particles;
    event.getByToken(m_packed_token, packed_gen_particles);

    for (const auto& p: *packed_gen_particles) {
        packed_p4.push_back(LorentzVector(p.pt(), p.eta(), p.phi(), p.energy()));
        packed_y.push_back(p.rapidity());
        packed_pdg_id.push_back(p.pdgId());
        packed_status.push_back(p.status());
        packed_status_flags.push_back(p.statusFlags().flags_.to_ulong());

        if (p.numberOfMothers() > 0) {
            packed_mothers_index.push_back({ static_cast<uint16_t>(p.motherRef().key()) });
        } else {
            packed_mothers_index.push_back({});
        }
    }

    edm::Handle<std::vector<reco::GenParticle>> pruned_gen_particles;
    event.getByToken(m_pruned_token, pruned_gen_particles);

    for (const auto& p: *pruned_gen_particles) {
        pruned_p4.push_back(LorentzVector(p.pt(), p.eta(), p.phi(), p.energy()));
        pruned_y.push_back(p.rapidity());
        pruned_pdg_id.push_back(p.pdgId());
        pruned_status.push_back(p.status());
        pruned_status_flags.push_back(p.statusFlags().flags_.to_ulong());

        std::vector<uint16_t> mothers_index;
        mothers_index.reserve(p.numberOfMothers());
        for (size_t i = 0; i < p.numberOfMothers(); i++)
            mothers_index.push_back(static_cast<uint16_t>(p.motherRef(i).key()));

        pruned_mothers_index.push_back(mothers_index);
    }
}
