
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(False, eras.Run2_25ns, globalTag='80X_mcRun2_asymptotic_2016_miniAODv2_v1')

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
# framework.applyMuonCorrection("kamuca")

#framework.doSystematics(['jec', 'jer'])

# Change the pt cut for testing if it propagates correctly
#framework.getProducer('jets').parameters.cut = 'pt > 50'

process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        '/store/mc/RunIISpring16MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/60000/022B4AD3-7A1B-E611-812B-28924A33B9AA.root'
        )

# Only run on a specific event. Useful for debugging

# NaN b-tagging discriminant for a jet
# input file: /store/mc/RunIISpring15MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/88E6468A-C56D-E511-B6C8-001E67248142.root
#process.source.eventsToProcess = cms.untracked.VEventRange(
        #'1:25002:4987798',
        #)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(400))
