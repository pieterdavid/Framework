
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(False, eras.Run2_25ns, globalTag='74X_mcRun2_asymptotic_v2')

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

#framework.redoJEC()
framework.smearJets()

framework.doSystematics(['jec', 'jer'])

# Change the pt cut for testing if it propagates correctly
framework.getProducer('jets').parameters.cut = 'pt > 50'

process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        '/store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/00000/0014DC94-DC5C-E511-82FB-7845C4FC39F5.root'
        )

print process.framework.producers.jets.parameters.cut
print process.framework.producers.jets_jecup.parameters.cut
print process.framework.producers.jets_jecdown.parameters.cut

# Only run on a specific event. Useful for debugging

# NaN b-tagging discriminant for a jet
# input file: /store/mc/RunIISpring15MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/88E6468A-C56D-E511-B6C8-001E67248142.root
#process.source.eventsToProcess = cms.untracked.VEventRange(
        #'1:25002:4987798',
        #)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(400))
