import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('gen_particles'),
        prefix = cms.string('gen_particle_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            )
        )
