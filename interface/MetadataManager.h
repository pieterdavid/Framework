#pragma once

#include <TObject.h>
#include <TFile.h>

#include <vector>
#include <memory>

class MetadataManager {

    public:
        MetadataManager(TFile* file):
            m_file(file) {
                // Empty
            }

        // Specialized into .cc
        template<typename T>
            void add(const std::string& name, const T& value);

    private:
        TFile* m_file;
        std::vector<std::shared_ptr<TObject>> m_trash;

};
