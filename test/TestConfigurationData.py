
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

process = Framework.create(True, eras.Run2_50ns, '74X_dataRun2_v2', redoJEC=True)

process.framework.analyzers.dilepton = cms.PSet(
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
        )

process.framework.analyzers.test = cms.PSet(
        type = cms.string('test_analyzer'),
        prefix = cms.string('test_'),
        enable = cms.bool(True)
        )
    
process.framework.analyzers.bTagsLoose = cms.PSet(
        type = cms.string('btags_analyzer'),
        prefix = cms.string('btags_CSVv2_loose'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            discr_name = cms.untracked.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'),
            discr_cut = cms.untracked.double(0.605),
            eta_cut = cms.untracked.double(2.4),
            pt_cut = cms.untracked.double(30)
            )
        )

process.framework.analyzers.bTagsMedium = cms.PSet(
        type = cms.string('btags_analyzer'),
        prefix = cms.string('btags_CSVv2_medium'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            discr_name = cms.untracked.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'),
            discr_cut = cms.untracked.double(0.89),
            eta_cut = cms.untracked.double(2.4),
            pt_cut = cms.untracked.double(30)
            )
        )

process.framework.analyzers.bTagsTight = cms.PSet(
        type = cms.string('btags_analyzer'),
        prefix = cms.string('btags_CSVv2_tight'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            discr_name = cms.untracked.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'),
            discr_cut = cms.untracked.double(0.97),
            eta_cut = cms.untracked.double(2.4),
            pt_cut = cms.untracked.double(30)
            )
        )

Framework.schedule(process, analyzers=['dilepton', 'bTagsLoose', 'bTagsMedium', 'bTagsTight', 'test'],
        producers=['event', 'hlt', 'vertices', 'electrons', 'muons', 'jets', 'fat_jets', 'met', 'nohf_met'])

process.source.fileNames = cms.untracked.vstring(
        '/store/data/Run2015B/DoubleMuon/MINIAOD/17Jul2015-v1/30000/D8ED75E7-C12E-E511-8CBF-0025905A608C.root'
        )
