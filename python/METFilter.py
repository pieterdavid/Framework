import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('met'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            # Note: HBHE noise filter is ran on-the-fly before the framework
            flags = cms.untracked.vstring('Flag_CSCTightHaloFilter', 'Flag_goodVertices', 'Flag_eeBadScFilter'),
            filters = cms.untracked.InputTag('TriggerResults', '', 'PAT')
            )
        )

