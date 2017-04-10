
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework
from cp3_llbb.Framework.CmdLine import CmdLine

options = CmdLine(defaults=dict(runOnData=1, era="25ns", globalTag='80X_dataRun2_Prompt_ICHEP16JEC_v0', process="RECO"))

framework = Framework.Framework(options)

framework.redoJEC()

process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        'file://DoubleMuon_Run2016B_PromptReco-v2_reduced.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
