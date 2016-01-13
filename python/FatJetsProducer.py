import copy

import FWCore.ParameterSet.Config as cms

from cp3_llbb.Framework import JetsProducer

# Clone standard jet configuration
default_configuration = copy.deepcopy(JetsProducer.default_configuration)

# And override some parameters
default_configuration.type = cms.string('fat_jets')
default_configuration.prefix = cms.string('fatjet_')
default_configuration.parameters.jets = cms.untracked.InputTag('slimmedJetsAK8')
default_configuration.parameters.cut = cms.untracked.string("pt > 150")
default_configuration.parameters.subjets_btags = cms.untracked.vstring('pfCombinedSecondaryVertexV2BJetTags', 'pfCombinedInclusiveSecondaryVertexV2BJetTags')
