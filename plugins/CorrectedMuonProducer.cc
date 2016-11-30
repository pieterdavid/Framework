#include <cp3_llbb/Framework/interface/CorrectedMuonProducerT.h>

#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/Muon.h>

typedef CorrectedMuonProducerT<reco::Muon, cp3::KaMuCaCorrector> KaMuCaCorrectedMuonProducer;
typedef CorrectedMuonProducerT<pat::Muon, cp3::KaMuCaCorrector> KaMuCaCorrectedPATMuonProducer;

typedef CorrectedMuonProducerT<reco::Muon, cp3::RochesterCorrector> RochesterCorrectedMuonProducer;
typedef CorrectedMuonProducerT<pat::Muon, cp3::RochesterCorrector> RochesterCorrectedPATMuonProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(KaMuCaCorrectedMuonProducer);
DEFINE_FWK_MODULE(KaMuCaCorrectedPATMuonProducer);
DEFINE_FWK_MODULE(RochesterCorrectedMuonProducer);
DEFINE_FWK_MODULE(RochesterCorrectedPATMuonProducer);
