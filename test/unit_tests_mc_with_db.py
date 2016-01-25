
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(False, eras.Run2_25ns, globalTag='76X_mcRun2_asymptotic_v12')
framework.redoJEC(JECDatabase='Summer15_25nsV5_MC.db')
process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        'file:///afs/cern.ch/user/s/sbrochet/public/CP3/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_PU25nsData2015v1_76X_reduced.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
