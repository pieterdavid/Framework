#ifndef EVENT_PRODUCER
#define EVENT_PRODUCER

#include <cp3_llbb/Framework/interface/Producer.h>
#include <cp3_llbb/Framework/interface/PUReweighter.h>

#include <SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h>
#include <SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h>
#include <SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h>

class EventProducer: public Framework::Producer {
    public:
        EventProducer(const std::string& name, const ROOT::TreeGroup& tree, const edm::ParameterSet& config):
            Producer(name, tree, config)
        {

            if (config.getUntrackedParameter<bool>("compute_pu_weights", true))
                m_pu_reweighter = std::make_shared<Framework::PUReweighter>(config.getParameterSet("pu_reweighter"), Framework::PUProfile::Run2015_25ns);

        }

        virtual ~EventProducer() {}

        virtual void doConsumes(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) override {
            m_rho_token = collector.consumes<double>(config.getUntrackedParameter<edm::InputTag>("rho", edm::InputTag("fixedGridRhoFastjetAll")));
            m_pu_info_token = collector.consumes<std::vector<PileupSummaryInfo>>(config.getUntrackedParameter<edm::InputTag>("pu_summary", edm::InputTag("addPileupInfo")));
            m_gen_info_token = collector.consumes<GenEventInfoProduct>(config.getUntrackedParameter<edm::InputTag>("gen_info", edm::InputTag("generator")));
            m_lhe_info_token = collector.consumes<LHEEventProduct>(config.getUntrackedParameter<edm::InputTag>("lhe_info", edm::InputTag("externalLHEProducer")));
        }

        virtual void produce(edm::Event& event, const edm::EventSetup& eventSetup) override;

        virtual void endJob(MetadataManager&) override;

    private:

        // Tokens
        edm::EDGetTokenT<double> m_rho_token;
        edm::EDGetTokenT<std::vector<PileupSummaryInfo>> m_pu_info_token;
        edm::EDGetTokenT<GenEventInfoProduct> m_gen_info_token;
        edm::EDGetTokenT<LHEEventProduct> m_lhe_info_token;

        float m_event_weight_sum = 0;

        std::shared_ptr<Framework::PUReweighter> m_pu_reweighter;

    public:
        // Tree members

        BRANCH(run, ULong64_t);
        BRANCH(lumi, ULong64_t);
        BRANCH(event, ULong64_t);

        BRANCH(rho, float);

        BRANCH(npu, int);
        BRANCH(true_interactions, float);
        BRANCH(pu_weight, float);

        BRANCH(pt_hat, float);
        BRANCH(weight, float);

        BRANCH(alpha_QCD, float);
        BRANCH(alpha_QED, float);
        BRANCH(q_scale, float);
        BRANCH(pdf_id, std::pair<int, int>);
        BRANCH(pdf_x, std::pair<float, float>);

        BRANCH(n_ME_partons, int);
        BRANCH(n_ME_partons_filtered, int);

        BRANCH(lhe_originalXWGTUP, float);
        BRANCH(lhe_SCALUP, float);
        BRANCH(lhe_weights, std::vector<std::pair<std::string, float>>);
};

#endif
