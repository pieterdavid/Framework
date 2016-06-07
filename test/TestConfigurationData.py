
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_25ns, globalTag='80X_dataRun2_Prompt_v8', processName='RECO')

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
        '/store/data/Run2016B/DoubleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/A6AC80E5-121A-E611-A689-02163E01439E.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
