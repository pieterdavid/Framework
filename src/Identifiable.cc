#include <cp3_llbb/Framework/interface/Identifiable.h>

void Identifiable::consumes_id_tokens(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) {
    if (config.existsAs<std::vector<edm::InputTag>>("ids")) {
        const std::vector<edm::InputTag>& id_tags = config.getUntrackedParameter<std::vector<edm::InputTag>>("ids");

        for (const edm::InputTag& tag: id_tags) {
            m_id_tokens.push_back(std::make_pair(tag.instance(), collector.consumes<edm::ValueMap<bool>>(tag)));
        }
    }
}

void Identifiable::retrieves_id_tokens(const edm::Event& event, const edm::EventSetup& eventSetup) {
    for (auto& token: m_id_tokens) {
        edm::Handle<edm::ValueMap<bool>> handle;
        event.getByToken(token.second, handle);
        m_id_maps.push_back(std::make_pair(token.first, handle));
    }
}
