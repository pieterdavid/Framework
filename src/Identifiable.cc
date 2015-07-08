#include <cp3_llbb/Framework/interface/Identifiable.h>
#include <iostream>

void Identifiable::consumes_id_tokens(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) {
    std::cout << "Entering Identifiable::consumes_id_tokens: is there any tracked vector of input tag: " << config.existsAs<std::vector<edm::InputTag>>("ids") << std::endl;
    std::cout << "Entering Identifiable::consumes_id_tokens: is there any untracked vector of input tag: " << config.existsAs<std::vector<edm::InputTag>>("ids", false) << std::endl;
    if (config.existsAs<std::vector<edm::InputTag>>("ids", false)) {
        const std::vector<edm::InputTag>& id_tags = config.getUntrackedParameter<std::vector<edm::InputTag>>("ids");
        std::cout << "\t and there exists id input tags: id_tags.size()= " << id_tags.size() << std::endl;
        for (const edm::InputTag& tag: id_tags) {
            std::cout << "tag.label()= " << tag.label() << std::endl;
            std::cout << "tag.instance()= " << tag.instance() << std::endl;
            m_id_tokens.push_back(std::make_pair(tag.instance(), collector.consumes<edm::ValueMap<bool>>(tag)));
        }
    }
}

void Identifiable::retrieves_id_tokens(const edm::Event& event, const edm::EventSetup& eventSetup) {
    std::cout << "Entering Identifiable::retrieves_id_tokens" << "\tm_id_tokens.size()= " << m_id_tokens.size() << std::endl;
    for (auto& token: m_id_tokens) {
//        std::cout << "\tm_id_tokens.size()= " << m_id_tokens.size() << std::endl;
        edm::Handle<edm::ValueMap<bool>> handle;
        event.getByToken(token.second, handle);
        m_id_maps.push_back(std::make_pair(token.first, handle));
    }
}
