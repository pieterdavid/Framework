#include <FWCore/PluginManager/interface/PluginFactory.h>

#include <cp3_llbb/Framework/interface/GenParticlesProducer.h>
#include <cp3_llbb/Framework/interface/JetsProducer.h>

DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, GenParticlesProducer, "gen_particles");
DEFINE_EDM_PLUGIN(ExTreeMakerProducerFactory, JetsProducer, "jets");
