import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('event'),
        prefix = cms.string('event_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            pu_summary = cms.untracked.InputTag('slimmedAddPileupInfo'),
            compute_pu_weights = cms.untracked.bool(True),
            pu_reweighter = cms.PSet(
                    data_pu_profile = cms.untracked.FileInPath('cp3_llbb/Framework/data/PUProfiles/pu_histogram_all_2015_69000minbias.root'),
                ),
            pu_reweighter_up = cms.PSet(
                    data_pu_profile = cms.untracked.FileInPath('cp3_llbb/Framework/data/PUProfiles/pu_histogram_all_2015_72450minbias.root')
                ),
            pu_reweighter_down = cms.PSet(
                    data_pu_profile = cms.untracked.FileInPath('cp3_llbb/Framework/data/PUProfiles/pu_histogram_all_2015_65550minbias.root')
                )
            )
        )
