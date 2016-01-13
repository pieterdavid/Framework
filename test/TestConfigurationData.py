
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_50ns, globalTag='74X_dataRun2_v2', processName='RECO')

framework.addAnalyzer('dilepton', cms.PSet(
        type = cms.string('dilepton_analyzer'),
        prefix = cms.string('dilepton_'),
        enable = cms.bool(True),
        categories_parameters = cms.PSet(
            mll_cut = cms.untracked.double(20)
            ),
        parameters = cms.PSet(
            standalone = cms.untracked.bool(True),
            muons_wp = cms.untracked.string('loose'),
            electrons_wp = cms.untracked.string('loose')
            )
        ))

framework.addAnalyzer('test', cms.PSet(
        type = cms.string('test_analyzer'),
        prefix = cms.string('test_'),
        enable = cms.bool(True)
        ))

framework.doSystematics(['jec', 'jer'])
    
process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        '/store/data/Run2015D/DoubleMuon/MINIAOD/PromptReco-v3/000/256/675/00000/4AA27F21-8B5F-E511-9AED-02163E014472.root'
        )
