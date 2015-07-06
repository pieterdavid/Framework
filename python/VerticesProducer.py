import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('vertices'),
        prefix = cms.string('vertex_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            )
        )
