
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(False, eras.Run2_25ns, globalTag='76X_mcRun2_asymptotic_v12')

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
    enable = cms.bool(True),
    parameters = cms.PSet(
        test = cms.untracked.string('jets'),
        test2 = cms.untracked.string('met')
        )
    ))

framework.removeProducer('fat_jets')

# Load JEC from the specified database instead of the GT. Will also affect the JEC systematics
#framework.useJECDatabase('Fall15_25nsV2_MC.db')

#framework.redoJEC()
framework.smearJets()

#framework.doSystematics(['jec', 'jer'])

# Change the pt cut for testing if it propagates correctly
#framework.getProducer('jets').parameters.cut = 'pt > 50'

process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        '/store/mc/RunIIFall15MiniAODv1/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/30000/2EE074B2-0EA2-E511-9505-44A842CFD619.root'
        )

# Only run on a specific event. Useful for debugging

# NaN b-tagging discriminant for a jet
# input file: /store/mc/RunIISpring15MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/88E6468A-C56D-E511-B6C8-001E67248142.root
#process.source.eventsToProcess = cms.untracked.VEventRange(
        #'1:25002:4987798',
        #)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(400))
