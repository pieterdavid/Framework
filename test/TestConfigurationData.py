
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_25ns, globalTag='80X_dataRun2_Prompt_v8', processName='reRECO')

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
        '/store/relval/CMSSW_8_0_5/DoubleMuon/MINIAOD/80X_dataRun2_relval_v9_RelVal_doubMu2015D-v1/00000/60218FF9-A008-E611-87E3-0CC47A4D7662.root'
        )
