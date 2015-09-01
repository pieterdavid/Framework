import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('electrons'),
        prefix = cms.string('electron_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            src = cms.untracked.InputTag('slimmedElectrons'),
            ea_R03 = cms.untracked.FileInPath('RecoEgamma/ElectronIdentification/data/PHYS14/effAreaElectrons_cone03_pfNeuHadronsAndPhotons.txt'),
            ea_R04 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaElectrons_cone04_pfNeuHadronsAndPhotons.txt'),
            ids = cms.untracked.VInputTag(
                'egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp80',
                'egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp90',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-veto',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-loose',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-medium',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V1-standalone-tight',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium',
                'egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight',
                'egmGsfElectronIDs:heepElectronID-HEEPV60'
                ),
            scale_factors = cms.untracked.PSet(
                id_veto = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_sfVETO_id.json'),
                id_loose = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_sfLOOSE_id.json'),
                id_medium = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_sfMEDIUM_id.json'),
                id_tight = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/Electron_sfTIGHT_id.json'),
                )
            ),
        )
