#include <cp3_llbb/Framework/interface/ScaleFactors.h>
#include <cp3_llbb/Framework/interface/BinnedValuesJSONParser.h>

#include <iostream>

void ScaleFactors::create_branches(const edm::ParameterSet& config) {

    if (config.existsAs<edm::ParameterSet>("scale_factors", false)) {
        const edm::ParameterSet& scale_factors = config.getUntrackedParameter<edm::ParameterSet>("scale_factors");
        std::vector<std::string> scale_factors_name = scale_factors.getParameterNames();

        for (const std::string& scale_factor: scale_factors_name) {
            create_branch(scale_factor, "sf_" + scale_factor);

            BinnedValuesJSONParser parser(scale_factors.getUntrackedParameter<edm::FileInPath>(scale_factor).fullPath());

            m_scale_factors.emplace(scale_factor, std::move(parser.get_values()));
        }
    }

}

void ScaleFactors::create_branch(const std::string& scale_factor, const std::string& branch_name) {
    // Default implementation. Just create the branch in the tree
    if (m_branches.count(scale_factor) == 0)
        m_branches.emplace(scale_factor, & m_tree[branch_name].write<std::vector<std::vector<float>>>());
}

void ScaleFactors::store_scale_factors(const std::vector<float>& values,bool isData) {
    for (const auto& sf: m_scale_factors) {
        if (isData)
            (*m_branches[sf.first]).push_back({1., 0., 0.});
        else
            (*m_branches[sf.first]).push_back(sf.second.get(values));
    }
}

float ScaleFactors::get_scale_factor(const std::string& name, size_t index, Variation variation/* = Variation::Nominal*/) {
    auto sf = m_branches.find(name);
    if (sf == m_branches.end())
        return 0;

    if (index >= sf->second->size())
        return 0;

    return (*sf->second)[index][static_cast<size_t>(variation)];
}
