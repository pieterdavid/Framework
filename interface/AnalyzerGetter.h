#pragma once

#include <string>

namespace Framework {
    class Analyzer;
};

class AnalyzerGetter {
    public:
        virtual const Framework::Analyzer& getAnalyzer(const std::string& name) const = 0;
        virtual bool analyzerExists(const std::string& name) const = 0;
};
