#include <cp3_llbb/Framework/interface/HLTService.h>
#include "tinyxml2.h"

#include <iostream>

using namespace tinyxml2;

bool HLTService::parse(const std::string& filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str())) {
        doc.PrintError();
        return false;
    }

    const XMLElement* root = doc.FirstChildElement("triggers");
    if (! root)
        return false;

    const XMLElement* runs = root->FirstChildElement("runs");
    for (; runs; runs = runs->NextSiblingElement("runs")) {
        parseRunsElement(runs);
    }

    return true;
}

bool HLTService::parseRunsElement(const XMLElement* runs) {
    Range<uint64_t> runRange(runs->UnsignedAttribute("from"), runs->UnsignedAttribute("to"));

    PathVector runPaths;

    const XMLElement* paths = runs->FirstChildElement("path");
    for (; paths; paths = paths->NextSiblingElement("path")) {
        const std::string name = paths->GetText();
        runPaths.push_back(PathName(name, boost::regex_constants::icase));
    }

    m_paths[runRange] = runPaths;
    return true;
}

const HLTService::PathVector& HLTService::getPaths(uint64_t run) {
    if (m_cachedRange && m_cachedRange->in(run)) {
        return *m_cachedVector;
    }

    for (auto& path: m_paths) {
        const auto& runRange = path.first;

        if (runRange.in(run)) {

            m_cachedRange = &runRange;
            m_cachedVector = &path.second;

            return *m_cachedVector;
        }
    }

    std::stringstream ss;
    ss << "Error: run " << run << " not found for triggers selection" << std::endl;
    throw std::logic_error(ss.str());
}

void HLTService::print() {
    for (auto& p: m_paths) {
        const auto& runRange = p.first;
        const auto& paths = p.second;

        std::cout << " -> Runs: " << runRange << std::endl;
        for (auto& path: paths) {
            std::cout << "    - " << path << std::endl;
        }
    }
}
