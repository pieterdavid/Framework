#include "cp3_llbb/Framework/interface/ShiftedJetProducerWithSourcesT.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

typedef ShiftedJetProducerWithSourcesT<pat::Jet> ShiftedPATJetProducerWithSources;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ShiftedPATJetProducerWithSources);
