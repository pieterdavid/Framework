import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('hlt'),
        prefix = cms.string('hlt_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            )
        )
