
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(False, eras.Run2_25ns, globalTag='80X_mcRun2_asymptotic_2016_miniAODv2')
framework.redoJEC(JECDatabase='Spring16_25nsV1_MC.db')
process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        'file:///afs/cern.ch/user/s/sbrochet/public/CP3/TTTo2L2Nu_13TeV-powheg_RunIISpring16MiniAODv2_reduced.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
