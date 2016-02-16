import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('met'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            flags = cms.untracked.vstring('Flag_HBHENoiseFilter', 'Flag_HBHENoiseIsoFilter', 'Flag_CSCTightHalo2015Filter', 'Flag_EcalDeadCellTriggerPrimitiveFilter', 'Flag_goodVertices', 'Flag_eeBadScFilter'),
            filters = cms.untracked.InputTag('TriggerResults', '', 'PAT')
            )
        )

