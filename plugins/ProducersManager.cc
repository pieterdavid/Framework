#include <cp3_llbb/Framework/interface/ProducersManager.h>

#include <cp3_llbb/Framework/plugins/Framework.h>
#include <cp3_llbb/Framework/interface/Producer.h>

ProducersManager::ProducersManager(ExTreeMaker* framework):
    m_framework(framework) {
        // Empty
}

const Framework::Producer& ProducersManager::get(const std::string& name) const {
    return m_framework->getProducer(name);
}
