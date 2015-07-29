import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('muons'),
        prefix = cms.string('muon_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            src = cms.untracked.InputTag('slimmedMuons'),
            ea_R03 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaMuons_cone03_pfNeuHadronsAndPhotons.txt'),
            ea_R04 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaMuons_cone04_pfNeuHadronsAndPhotons.txt'),
            scale_factors = cms.untracked.PSet(
                id_loose = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_Loose_id.json'),
                id_soft = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_Soft_id.json'),
                id_tight = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_Tight_id.json'),

                iso_012_tight = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_combRelIsoPF04dBeta<012_Tight_iso.json'),
                iso_02_loose = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_combRelIsoPF04dBeta<02_Loose_iso.json'),
                iso_02_tight = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_combRelIsoPF04dBeta<02_Tight_iso.json'),
                )
            )
        )
