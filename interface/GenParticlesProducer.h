#ifndef GENPARTICLES_PRODUCER
#define GENPARTICLES_PRODUCER

#include <DataFormats/PatCandidates/interface/PackedGenParticle.h>
#include <DataFormats/HepMCCandidate/interface/GenParticle.h>

#include <cp3_llbb/Framework/interface/Producer.h>

class GenParticlesProducer: public Framework::Producer {
    public:
        GenParticlesProducer(const std::string& name, const ROOT::TreeGroup& tree, const edm::ParameterSet& config):
            Producer(name, tree, config),
            tree_packed(tree.group("packed_")),
            tree_pruned(tree.group("pruned_"))
        {

        }

        virtual void doConsumes(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) override {
            m_packed_token = collector.consumes<std::vector<pat::PackedGenParticle>>(config.getUntrackedParameter<edm::InputTag>("packed_gen_particles", edm::InputTag("packedGenParticles")));
            m_pruned_token = collector.consumes<std::vector<reco::GenParticle>>(config.getUntrackedParameter<edm::InputTag>("pruned_gen_particles", edm::InputTag("prunedGenParticles")));
        }

        virtual ~GenParticlesProducer() {}

        virtual void produce(edm::Event& event, const edm::EventSetup& eventSetup) override;

    private:

        // Tokens
        edm::EDGetTokenT<std::vector<pat::PackedGenParticle>> m_packed_token;
        edm::EDGetTokenT<std::vector<reco::GenParticle>> m_pruned_token;

        ROOT::TreeGroup tree_packed;
        ROOT::TreeGroup tree_pruned;

    public:
        // Tree members
        std::vector<LorentzVector>& packed_p4 = tree_packed["p4"].write<std::vector<LorentzVector>>();
        std::vector<float>& packed_y = tree_packed["y"].write<std::vector<float>>();
        std::vector<int16_t>& packed_pdg_id = tree_packed["pdg_id"].write<std::vector<int16_t>>();
        std::vector<int8_t>& packed_status = tree_packed["status"].write<std::vector<int8_t>>();
        std::vector<int16_t>& packed_status_flags = tree_packed["status_flags"].write<std::vector<int16_t>>();
        std::vector<std::vector<uint16_t>>& packed_mothers_index = tree_packed["mothers_index"].write<std::vector<std::vector<uint16_t>>>();

        std::vector<LorentzVector>& pruned_p4 = tree_pruned["p4"].write<std::vector<LorentzVector>>();
        std::vector<float>& pruned_y = tree_pruned["y"].write<std::vector<float>>();
        std::vector<int16_t>& pruned_pdg_id = tree_pruned["pdg_id"].write<std::vector<int16_t>>();
        std::vector<int8_t>& pruned_status = tree_pruned["status"].write<std::vector<int8_t>>();
        std::vector<int16_t>& pruned_status_flags = tree_pruned["status_flags"].write<std::vector<int16_t>>();
        std::vector<std::vector<uint16_t>>& pruned_mothers_index = tree_pruned["mothers_index"].write<std::vector<std::vector<uint16_t>>>();
};

#endif

