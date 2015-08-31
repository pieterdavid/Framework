import FWCore.ParameterSet.Config as cms

default_configuration = cms.PSet(
        type = cms.string('jets'),
        prefix = cms.string('jet_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            btags = cms.untracked.vstring('pfCombinedInclusiveSecondaryVertexV2BJetTags', 'pfJetProbabilityBJetTags', 'pfCombinedMVABJetTags'),
            scale_factors = cms.untracked.PSet(
                csv_loose = cms.untracked.PSet(
                    algorithm = cms.untracked.string('csv'),
                    working_point = cms.untracked.string('loose'),
                    files = cms.untracked.VPSet(
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('bjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_loose_bjets_comb_csv.json'),
                            ),
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('cjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_loose_cjets_comb_csv.json'),
                            ),
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('lightjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_loose_lightjets_comb_csv.json'),
                            ),
                        )
                    ),
                csv_medium = cms.untracked.PSet(
                    algorithm = cms.untracked.string('csv'),
                    working_point = cms.untracked.string('medium'),
                    files = cms.untracked.VPSet(
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('bjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_medium_bjets_comb_csv.json'),
                            ),
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('cjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_medium_cjets_comb_csv.json'),
                            ),
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('lightjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_medium_lightjets_comb_csv.json'),
                            ),
                        )
                    ),
                csv_tight = cms.untracked.PSet(
                    algorithm = cms.untracked.string('csv'),
                    working_point = cms.untracked.string('tight'),
                    files = cms.untracked.VPSet(
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('bjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_tight_bjets_comb_csv.json'),
                            ),
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('cjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_tight_cjets_comb_csv.json'),
                            ),
                        cms.untracked.PSet(
                            flavor = cms.untracked.string('lightjets'),
                            file = cms.untracked.FileInPath('cp3_llbb/Framework/data/ScaleFactors/BTagging_tight_lightjets_comb_csv.json'),
                            ),
                        )
                    )
                )
            )
        )
