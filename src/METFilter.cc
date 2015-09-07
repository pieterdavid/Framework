#include <cp3_llbb/Framework/interface/METFilter.h>

#include <FWCore/Common/interface/TriggerNames.h>

bool METFilter::filter(edm::Event& event, const edm::EventSetup& eventSetup) {
    edm::Handle<edm::TriggerResults> filters;
    event.getByToken(m_met_filters_token, filters);

    const edm::TriggerNames& triggerNames = event.triggerNames(*filters);

    for (size_t i = 0 ; i < filters->size(); i++) {
        std::string triggerName = triggerNames.triggerName(i);
        if (std::find(m_flags.begin(), m_flags.end(), triggerName) != m_flags.end()) {
            if (!filters->accept(i))
                return false;
        } else {
            continue;
        }
    }

    return true;
}
