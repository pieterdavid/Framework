
#include <cp3_llbb/Framework/interface/HLTProducer.h>

void HLTProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    edm::Handle<edm::TriggerResults> hlt;
    event.getByToken(m_hlt_token, hlt);

    edm::Handle<pat::PackedTriggerPrescales> prescales_;
    event.getByToken(m_prescales_token, prescales_);

    const edm::TriggerNames& triggerNames = event.triggerNames(*hlt);

    for (size_t i = 0 ; i < hlt->size(); i++) {
        if (hlt->accept(i)) {
            std::string triggerName = triggerNames.triggerName(i);
            if (triggerName == "HLTriggerFinalPath")
                continue; // This one is pretty useless...
            if (triggerName[0] == 'A')
                continue; // Remove AlCa HLT paths

            paths.push_back(triggerName);
            if (prescales_.isValid()) {
                prescales.push_back(prescales_->getPrescaleForIndex(i));
            }
        }
    }
}
