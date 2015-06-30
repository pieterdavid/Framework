
import FWCore.ParameterSet.Config as cms

process = cms.Process("ETM")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.GlobalTag.globaltag = "MCRUN2_74_V7"

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5))
process.source = cms.Source("PoolSource")
process.source.fileNames = cms.untracked.vstring(
        'file:///home/fynu/sbrochet/storage/MINIAODSIM/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_Asympt25ns_MCRUN2_74_V9_reduced.root'
        )

# Services
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.framework = cms.EDProducer("ExTreeMaker",
        output = cms.string('output_mc.root'),

        producers = cms.PSet(

            jets = cms.PSet(
                type = cms.string('jets'),
                prefix = cms.string('jet_'),
                enable = cms.bool(True),
                parameters = cms.PSet(
                    cut = cms.untracked.string("pt > 10")
                    )
                ),

            gen_particles = cms.PSet(
                type = cms.string('gen_particles'),
                prefix = cms.string('gen_particle_'),
                enable = cms.bool(True)
                )

            ),

        analyzers = cms.PSet(
            test = cms.PSet(
                type = cms.string('test_analyzer'),
                prefix = cms.string('test_'),
                enable = cms.bool(True)
                )
            )
        )

process.p = cms.Path(process.framework)


#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.options.allowUnscheduled = cms.untracked.bool(True)
