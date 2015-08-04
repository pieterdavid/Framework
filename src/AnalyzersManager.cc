#include <cp3_llbb/Framework/interface/AnalyzersManager.h>

AnalyzersManager::AnalyzersManager(const AnalyzerGetter& getter):
    m_getter(getter) {
        // Empty
}

bool AnalyzersManager::exists(const std::string& name) const {
    return m_getter.analyzerExists(name);
}
