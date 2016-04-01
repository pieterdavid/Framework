import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('muons'),
        prefix = cms.string('muon_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            src = cms.untracked.InputTag('slimmedMuons'),
            applyRochester = cms.untracked.bool(False),
            ea_R03 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaMuons_cone03_pfNeuHadronsAndPhotons.txt'),
            ea_R04 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaMuons_cone04_pfNeuHadronsAndPhotons.txt'),
            scale_factors = cms.untracked.PSet(
                id_soft  = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_SoftID_genTracks_id.json'),
                id_loose  = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_LooseID_genTracks_id.json'),
                id_medium = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_MediumID_genTracks_id.json'),
                id_tight  = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_TightIDandIPCut_genTracks_id.json'),

                iso_loose_id_loose = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_LooseRelIso_LooseID_iso.json'),
                iso_loose_id_medium = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_LooseRelIso_MediumID_iso.json'),
                iso_loose_id_tight = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_LooseRelIso_TightID_iso.json'),
                iso_tight_id_medium = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_TightRelIso_MediumID_iso.json'),
                iso_tight_id_tight = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_TightRelIso_TightID_iso.json'),

                id_hww = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_MediumID_Data_MC_25ns_PTvsETA_HWW_76.json'),
                iso_tight_id_hww = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Muon_ISOTight_Data_MC_25ns_PTvsETA_HWW.json'),
                )
            )
        )
