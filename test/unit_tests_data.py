
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_25ns, globalTag='80X_dataRun2_Prompt_v8', processName='reRECO')

framework.redoJEC()

process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        'file:///afs/cern.ch/user/s/sbrochet/public/CP3/DoubleMuon-80X_dataRun2_relval_v9_RelVal-MiniAOD_reduced.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
