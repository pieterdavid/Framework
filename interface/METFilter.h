#pragma once

#include <cp3_llbb/Framework/interface/Filter.h>
#include <DataFormats/Common/interface/TriggerResults.h>

class METFilter: public Framework::Filter {
    public:
        METFilter(const std::string& name, const edm::ParameterSet& config):
            Filter(name, config)
        {
            m_flags = config.getUntrackedParameter<std::vector<std::string>>("flags");
        }

        virtual ~METFilter() {}

        virtual void doConsumes(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) override {
            m_met_filters_token = collector.consumes<edm::TriggerResults>(config.getUntrackedParameter<edm::InputTag>("filters", edm::InputTag("TriggerResults", "", "PAT")));
        }

        virtual bool filter(edm::Event& event, const edm::EventSetup& eventSetup) override;

    private:

        // Tokens
        edm::EDGetTokenT<edm::TriggerResults> m_met_filters_token;

        std::vector<std::string> m_flags;
};
