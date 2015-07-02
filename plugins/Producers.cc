#include <FWCore/PluginManager/interface/PluginFactory.h>

#include <cp3_llbb/Framework/interface/GenParticlesProducer.h>
#include <cp3_llbb/Framework/interface/HLTProducer.h>
#include <cp3_llbb/Framework/interface/JetsProducer.h>
#include <cp3_llbb/Framework/interface/METProducer.h>
#include <cp3_llbb/Framework/interface/MuonsProducer.h>
#include <cp3_llbb/Framework/interface/ElectronsProducer.h>

DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, GenParticlesProducer, "gen_particles");
DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, HLTProducer, "hlt");
DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, JetsProducer, "jets");
DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, METProducer, "met");
DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, MuonsProducer, "muons");
DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, ElectronsProducer, "electrons");
