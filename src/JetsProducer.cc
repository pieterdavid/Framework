
#include <cp3_llbb/Framework/interface/JetsProducer.h>

void JetsProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    edm::Handle<std::vector<pat::Jet>> jets;
    event.getByToken(m_jets_token, jets);

    for (const auto& jet: *jets) {
        if (! pass_cut(jet))
            continue;

        fill_candidate(jet);
    }
}
