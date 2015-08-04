#ifndef ANALYZERS_MANAGER
#define ANALYZERS_MANAGER

#include "cp3_llbb/Framework/interface/AnalyzerGetter.h"
#include "cp3_llbb/Framework/interface/Analyzer.h"

#include <string>
#include <type_traits>

class AnalyzersManager {
    friend class ExTreeMaker;

    public:
        template <class T>
            const T& get(const std::string& name) const {
                static_assert(std::is_base_of<Framework::Analyzer, T>::value, "T must inherit from Framework::Analyzer");
                return dynamic_cast<const T&>(m_getter.getAnalyzer(name));
            }

        bool exists(const std::string& name) const;

    private:
        AnalyzersManager(const AnalyzerGetter& getter);
        const AnalyzerGetter& m_getter;

};

#endif
