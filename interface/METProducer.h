#ifndef MET_PRODUCER
#define MET_PRODUCER

#include <DataFormats/PatCandidates/interface/MET.h>

#include <cp3_llbb/Framework/interface/Producer.h>

class METProducer: public Framework::Producer {
    public:
        METProducer(const std::string& name, const ROOT::TreeGroup& tree, const edm::ParameterSet& config):
            Producer(name, tree, config)
        {

        }

        virtual ~METProducer() {}

        virtual void doConsumes(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) override {
            m_met_token = collector.consumes<std::vector<pat::MET>>(config.getUntrackedParameter<edm::InputTag>("met", edm::InputTag("slimmedMETs")));
        }

        virtual void produce(edm::Event& event, const edm::EventSetup& eventSetup) override;

    private:

        // Tokens
        edm::EDGetTokenT<std::vector<pat::MET>> m_met_token;

    public:
        // Tree members
        LorentzVector& p4 = tree["p4"].write<LorentzVector>();
        float& sumEt = tree["sumEt"].write<float>();

        float& uncorrectedPt = tree["uncorrectedPt"].write<float>();
        float& uncorrectedPhi = tree["uncorrectedPhi"].write<float>();
        float& uncorrectedSumEt = tree["uncorrectedSumEt"].write<float>();
};

#endif
