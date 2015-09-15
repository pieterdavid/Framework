#include <FWCore/PluginManager/interface/PluginFactory.h>

#include <cp3_llbb/Framework/interface/DileptonAnalyzer.h>
#include <cp3_llbb/Framework/interface/BTagsAnalyzer.h>
#include <cp3_llbb/Framework/interface/TestAnalyzer.h>

DEFINE_EDM_PLUGIN(ExTreeMakerAnalyzerFactory, DileptonAnalyzer, "dilepton_analyzer");
DEFINE_EDM_PLUGIN(ExTreeMakerAnalyzerFactory, BTagsAnalyzer, "btags_analyzer");
DEFINE_EDM_PLUGIN(ExTreeMakerAnalyzerFactory, TestAnalyzer, "test_analyzer");
