
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_25ns, globalTag='80X_dataRun2_2016SeptRepro_v6', processName='RECO')

framework.addAnalyzer('dilepton', cms.PSet(
        type = cms.string('dilepton_analyzer'),
        prefix = cms.string('dilepton_'),
        enable = cms.bool(True),
        categories_parameters = cms.PSet(
            mll_cut = cms.untracked.double(20)
            ),
        parameters = cms.PSet(
            standalone = cms.untracked.bool(True),
            muons_wp = cms.untracked.string('loose'),
            electrons_wp = cms.untracked.string('loose')
            )
        ))

framework.addAnalyzer('test', cms.PSet(
        type = cms.string('test_analyzer'),
        prefix = cms.string('test_'),
        enable = cms.bool(True)
        ))

framework.applyElectronRegression()
framework.applyElectronSmearing()

framework.doSystematics(['jec', 'jer'])
    
process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        '/store/data/Run2016F/DoubleMuon/MINIAOD/23Sep2016-v1/50000/040EDEBA-0490-E611-A424-008CFA110C68.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
