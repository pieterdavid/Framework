#include <cp3_llbb/Framework/interface/ProducersManager.h>

ProducersManager::ProducersManager(const ProducerGetter& getter):
    m_getter(getter) {
        // Empty
}

bool ProducersManager::exists(const std::string& name) const {
    return m_getter.producerExists(name);
}
