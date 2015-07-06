import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('event'),
        prefix = cms.string('event_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            )
        )
