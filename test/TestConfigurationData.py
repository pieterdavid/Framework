
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework
from cp3_llbb.Framework.CmdLine import CmdLine

options = CmdLine(defaults=dict(runOnData=1, era="2017", globalTag='94X_dataRun2_ReReco_EOY17_v6', process='RECO'))

framework = Framework.Framework(options)

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

#framework.applyElectronRegression()
#framework.applyElectronSmearing(tag='Moriond17_23Jan')

from cp3_llbb.Framework.JetsProducer import discriminators_deepFlavour
framework.redoJEC(addBtagDiscriminators=discriminators_deepFlavour)
framework.doSystematics(['jec', 'jer'])
    
process = framework.create()

process.source.fileNames = cms.untracked.vstring(
        '/store/data/Run2017C/DoubleMuon/MINIAOD/31Mar2018-v1/00000/1E30A24E-8E39-E811-8AD3-8CDCD4A99E08.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
