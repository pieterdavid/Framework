#ifndef CUT_H
#define CUT_H

#include <string>
#include <map>

#include <cp3_llbb/TreeWrapper/interface/TreeWrapper.h>


struct CategoryData;

struct Cut {
    Cut(const std::string& name_, const std::string& description_, ROOT::TreeGroup& tree):
        cut(tree[name_ + "_cut"].write<bool>()),
        name(name_),
        description(description_) {

        // Empty
    }

    bool& cut;

    std::string name;
    std::string description;
};

class CutManager {
    public:
        CutManager(CategoryData& category):
            m_category(category)
        {
            // Empty
        }

        void new_cut(const std::string& name, const std::string& description);
        void pass_cut(const std::string& name);

    private:
        CategoryData& m_category;
        std::map<std::string, Cut> m_cuts;
};

#endif
