#ifndef HLT_PRODUCER
#define HLT_PRODUCER

#include <cp3_llbb/Framework/interface/CandidatesProducer.h>

#include <FWCore/Common/interface/TriggerNames.h>
#include <DataFormats/Common/interface/TriggerResults.h>
#include <DataFormats/PatCandidates/interface/PackedTriggerPrescales.h>

class HLTProducer: public Framework::Producer {
    public:
        HLTProducer(const std::string& name, const ROOT::TreeGroup& tree, const edm::ParameterSet& config):
            Producer(name, tree, config)
        {

        }

        virtual ~HLTProducer() {}

        virtual void doConsumes(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) override {
            m_hlt_token = collector.consumes<edm::TriggerResults>(config.getUntrackedParameter<edm::InputTag>("hlt", edm::InputTag("TriggerResults", "", "HLT")));
            m_prescales_token = collector.consumes<pat::PackedTriggerPrescales>(config.getUntrackedParameter<edm::InputTag>("prescales", edm::InputTag("patTrigger")));
        }

        virtual void produce(edm::Event& event, const edm::EventSetup& eventSetup) override;

    private:

        // Tokens
        edm::EDGetTokenT<edm::TriggerResults> m_hlt_token;
        edm::EDGetTokenT<pat::PackedTriggerPrescales> m_prescales_token;

    public:
        // Tree members
        std::vector<std::string>& paths = tree["paths"].write<std::vector<std::string>>();
        std::vector<uint16_t>& prescales = tree["prescales"].write<std::vector<uint16_t>>();
};

#endif
