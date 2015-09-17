#include <cp3_llbb/Framework/interface/Cut.h>
#include <cp3_llbb/Framework/interface/Category.h>

void CutManager::new_cut(const std::string& name, const std::string& description) {
    if (m_cuts.count(name) > 0) {
        throw edm::Exception(edm::errors::InsertFailure, "A cut named '" + name + "' already exists.");
    }
    m_cuts.emplace(name, Cut(name, description, m_category.tree));
}

void CutManager::pass_cut(const std::string& name) {

    auto cut = m_cuts.find(name);
    if (cut != m_cuts.end())
        cut->second.cut = true;
}

bool CutManager::cut_passed(const std::string& name) const {

    const auto cut = m_cuts.find(name);
    if (cut != m_cuts.end())
        return cut->second.cut;

    return false;
}
