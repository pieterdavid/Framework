
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_25ns, globalTag='76X_dataRun2_v15', processName='RECO')

framework.redoJEC()

process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        'file:///afs/cern.ch/user/s/sbrochet/public/CP3/DoubleMuonLowMass_Run2015D_16Dec2015_76X_reduced.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
