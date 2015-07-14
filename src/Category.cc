#include <cp3_llbb/Framework/interface/Category.h>

#include <cstdio>
#include <cinttypes>

void CategoryManager::new_category(const std::string& name, const std::string& description, Category* category) {
    m_categories.push_back(CategoryData(name, description, category, m_tree));
}

bool CategoryManager::evaluate(const ProducersManager& producers) {
    bool ret;

    if (m_categories.size() == 0) {
        ret = true;
    } else {
        ret = false;
        for (auto& category: m_categories) {
            if (category.callback->event_in_category(producers)) {
                ret = true;
                category.in_category = true;
                category.events++;
                category.callback->evaluate_cuts(category.cut_manager);
            }
        }
    }

    processed_events++;
    if (ret)
        selected_events++;

    return ret;
}

void CategoryManager::print_summary() {

    printf("%" PRIu64 " processed events, %" PRIu64 " selected.\n", processed_events, selected_events);

    if (m_categories.size() == 0)
        return;

    printf("%-60s %20s\n", "Category", "# events");
    printf("---------------------------------------------------------------------------------\n");
    for (auto& category: m_categories)
        printf("%-60s %20" PRIu64 "\n", category.name.c_str(), category.events);
}

