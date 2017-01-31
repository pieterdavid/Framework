import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('electrons'),
        prefix = cms.string('electron_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            src = cms.untracked.InputTag('slimmedElectrons'),
            ea_R03 = cms.untracked.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt'),
            ea_R04 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaElectrons_cone04_pfNeuHadronsAndPhotons.txt'),
            ids = cms.untracked.VInputTag(
                'egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto',
                'egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose',
                'egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium',
                'egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight',
                'egmGsfElectronIDs:cutBasedElectronHLTPreselection-Summer16-V1',
                'egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90',
                'egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80'
                ),
            mva_id = cms.untracked.PSet(
                values = cms.untracked.InputTag('electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values'),
                categories = cms.untracked.InputTag('electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Categories')
            ),
            scale_factors = cms.untracked.PSet(
                id_veto_ichep16 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_veto.json'),
                id_loose_ichep16 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_loose.json'),
                id_medium_ichep16 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_medium.json'),
                id_tight_ichep16 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_tight.json'),
                reco_ichep16 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_gsfTracking.json'),

                id_veto_moriond17 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_veto_moriond17.json'),
                id_loose_moriond17 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_loose_moriond17.json'),
                id_medium_moriond17 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_medium_moriond17.json'),
                id_tight_moriond17 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_tight_moriond17.json'),
                reco_moriond17 = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_EGamma_SF2D_reco_moriond17.json'),

                hww_wp = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_Tight_Run2016_Run273726to276811_HWW_weighted.json')
                )
            ),
        )
