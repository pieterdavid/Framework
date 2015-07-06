import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('electrons'),
        prefix = cms.string('electron_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            src = cms.untracked.InputTag('slimmedElectrons'),
            ea_R03 = cms.untracked.FileInPath('RecoEgamma/ElectronIdentification/data/PHYS14/effAreaElectrons_cone03_pfNeuHadronsAndPhotons.txt'),
            ea_R04 = cms.untracked.FileInPath('cp3_llbb/Framework/data/effAreaElectrons_cone04_pfNeuHadronsAndPhotons.txt'),
            ids = cms.untracked.vstring('mvaEleID-PHYS14-PU20bx25-nonTrig-V1-wp80', 'mvaEleID-PHYS14-PU20bx25-nonTrig-V1-wp90', 'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-loose', 'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium', 'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-tight', 'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-veto')
            )
        )
