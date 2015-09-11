import FWCore.ParameterSet.Config as cms

def setup_jets_(process, isData, bTagDiscriminators):
    from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
    jetToolbox(process, 'ak4', 'ak4CHSJetSequence', 'out', PUMethod='CHS', runOnMC=not isData, miniAOD=True, addPUJetID=False, bTagDiscriminators=bTagDiscriminators + ['pfSimpleSecondaryVertexHighEffBJetTags'])
    process.softPFElectronsTagInfosAK4PFCHS.electrons = cms.InputTag('slimmedElectrons')
    process.softPFMuonsTagInfosAK4PFCHS.muons = cms.InputTag('slimmedMuons')

    # b-tagging information. From
    # https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/PhysicsTools/PatAlgos/python/slimming/miniAOD_tools.py#L130
    process.patJetsAK4PFCHS.userData.userFunctions = cms.vstring(
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).p4.M):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).numberOfSourceCandidatePtrs):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").flightDistance(0).value):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").flightDistance(0).significance):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).p4.x):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).p4.y):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).p4.z):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).vertex.x):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).vertex.y):(0)',
            '?(tagInfoCandSecondaryVertex("pfSecondaryVertex").nVertices()>0)?(tagInfoCandSecondaryVertex("pfSecondaryVertex").secondaryVertex(0).vertex.z):(0)',
            )
    process.patJetsAK4PFCHS.userData.userFunctionLabels = cms.vstring('vtxMass','vtxNtracks','vtx3DVal','vtx3DSig','vtxPx','vtxPy','vtxPz','vtxPosX','vtxPosY','vtxPosZ')
    process.patJetsAK4PFCHS.tagInfoSources = cms.VInputTag(cms.InputTag("pfSecondaryVertexTagInfosAK4PFCHS"))
    process.patJetsAK4PFCHS.addTagInfos = cms.bool(True)

    # Pile-up jet id
    process.load('RecoJets.JetProducers.PileupJetID_cfi')
    process.pileupJetId.applyJec = False
    process.pileupJetId.vertexes = cms.InputTag('offlineSlimmedPrimaryVertices')
    process.patJetsAK4PFCHS.userData.userFloats.src = [ cms.InputTag("pileupJetId:fullDiscriminant"), ]


def setup_met_(process, isData):
    from PhysicsTools.PatAlgos.tools.metTools import addMETCollection

    ## Gen MET
    ### Copied from https://github.com/cms-sw/cmssw/blob/2b75137e278b50fc967f95929388d430ef64710b/RecoMET/Configuration/python/GenMETParticles_cff.py#L37
    process.genParticlesForMETAllVisible = cms.EDProducer(
            "InputGenJetsParticleSelector",
            src = cms.InputTag("prunedGenParticles"),
            partonicFinalState = cms.bool(False),
            excludeResonances = cms.bool(False),
            excludeFromResonancePids = cms.vuint32(),
            tausAsJets = cms.bool(False),

            ignoreParticleIDs = cms.vuint32(
                1000022,
                1000012, 1000014, 1000016,
                2000012, 2000014, 2000016,
                1000039, 5100039,
                4000012, 4000014, 4000016,
                9900012, 9900014, 9900016,
                39, 12, 14, 16
                )
            )
    process.load('RecoMET.METProducers.genMetTrue_cfi')

    # MET is done from all PF candidates, and Type-I corrections are computed from non-CHS ak4 PF jets

    ## Run AK4 PF jets without CHS
    from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
    jetToolbox(process, 'ak4', 'ak4JetSequence', 'out', PUMethod='Plain', runOnMC=not isData, miniAOD=True, addPUJetID=False, bTagDiscriminators=['jetProbabilityBJetTags'])

    ## Raw PF METs
    process.load('RecoMET.METProducers.PFMET_cfi')

    process.pfMet.src = cms.InputTag('packedPFCandidates')
    addMETCollection(process, labelName='patPFMet', metSource='pfMet') # RAW MET
    process.patPFMet.addGenMET = False

    ## Type 1 corrections
    process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
    from JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff import corrPfMetType1
    from JetMETCorrections.Type1MET.correctedMet_cff import pfMetT1

    if not hasattr(process, 'ak4PFJets'):
        print("WARNING: No AK4 jets produced. Type 1 corrections for MET are not available.")
    else:
        process.corrPfMetType1 = corrPfMetType1.clone(
            src = 'ak4PFJets',
            jetCorrLabel = 'ak4PFL1FastL2L3Corrector' if not isData else 'ak4PFL1FastL2L3ResidualCorrector',
            offsetCorrLabel = 'ak4PFL1FastjetCorrector'
        )
        process.pfMetT1 = pfMetT1.clone(
            src = 'pfMet',
            srcCorrections = [cms.InputTag("corrPfMetType1", "type1")]
        )

        addMETCollection(process, labelName='patMET', metSource='pfMetT1') # T1 MET
        process.patMET.addGenMET = False

    ## Slimmed METs

    from PhysicsTools.PatAlgos.slimming.slimmedMETs_cfi import slimmedMETs
    #### CaloMET is not available in MiniAOD
    del slimmedMETs.caloMET

    process.slimmedMETs = slimmedMETs.clone()
    if hasattr(process, "patMET"):
        # Create MET from Type 1 PF collection
        process.patMET.addGenMET = not isData
        process.slimmedMETs.src = cms.InputTag("patMET")
        process.slimmedMETs.rawUncertainties = cms.InputTag("patPFMet") # only central value
    else:
        # Create MET from RAW PF collection
        process.patPFMet.addGenMET = not isData
        process.slimmedMETs.src = cms.InputTag("patPFMet")
        del process.slimmedMETs.rawUncertainties # not available

    del process.slimmedMETs.type1Uncertainties # not available
    del process.slimmedMETs.type1p2Uncertainties # not available


def create(era, globalTag=None, analyzers=cms.PSet(), redoJEC=False):
    """Create the CMSSW python configuration for the Framework

    Args:
        era (Configuration.StandardSequences.Eras.eras): The era of the data sample. Use ``None`` for simulated samples
        globalTag (str): The global tag to use for this workflow. If set to ``None``, a command-line argument named ``globalTag`` must be specified
        analyzers (cms.PSet()): A list of analyzers to run. By default, it's empty, and you can still add one to the list afterwards.
        redoJEC (bool): If True, a new jet collection will be created, starting from MiniAOD jets but with latest JEC, pulled from the global tag.

    Returns:
        The ``process`` object for the CMSSW framework

        You can use the returned object to add / remove producers, analyzers from the default framework configuration.

    Examples:

        import FWCore.ParameterSet.Config as cms
        from Configuration.StandardSequences.Eras import eras

        from cp3_llbb.Framework import Framework

        # Run on 50ns data sample
        process = Framework.create(eras.Run2_50ns, '74X_dataRun2_Prompt_v1')

        # Run on 25ns data sample
        process = Framework.create(eras.Run2_25ns, '74X_dataRun2_Prompt_v2')

        # For MC
        process = Framework.create(None, 'MCRUN2_74_V9', cms.PSet(
                test = cms.PSet(
                    type = cms.string('test_analyzer'),
                    prefix = cms.string('test_'),
                    enable = cms.bool(True)
                    )
                )
            )
    """

    import sys
    for argv in sys.argv:
        if 'globalTag' in argv:

            from FWCore.ParameterSet.VarParsing import VarParsing
            options = VarParsing()
            options.register('globalTag',
                    '',
                    VarParsing.multiplicity.singleton,
                    VarParsing.varType.string,
                    'The globaltag to use')

            options.parseArguments()
            globalTag = options.globalTag

            break

    if globalTag is None:
        raise Exception("No global tag specified. Use the 'globalTag' command line argument to set one.\n\tcmsRun <configuration> --globalTag=<global_tag>")

    if era is not None:
        process = cms.Process("ETM", era)
    else:
        process = cms.Process("ETM")

    process.options = cms.untracked.PSet(
            wantSummary = cms.untracked.bool(False),
            allowUnscheduled = cms.untracked.bool(True)
            )

    isData = era is not None

    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
    process.load("Configuration.EventContent.EventContent_cff")
    process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
    process.load('Configuration.StandardSequences.MagneticField_38T_cff')

    process.GlobalTag.globaltag = globalTag

    process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(20))
    process.source = cms.Source("PoolSource")

    # Electron ID recipe
    from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat, switchOnVIDElectronIdProducer, setupAllVIDIdsInModule, setupVIDElectronSelection
    dataFormat = DataFormat.MiniAOD
    switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

    id_modules = [
            'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_50ns_V1_cff',
            'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff',
            'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_nonTrig_V1_cff',
            'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff'
            ]
    for idmod in id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)

    # Services
    process.load('FWCore.MessageLogger.MessageLogger_cfi')
    process.MessageLogger.cerr.FwkReport.reportEvery = 100

    bTagDiscriminators = [
            'pfCombinedInclusiveSecondaryVertexV2BJetTags', 'pfJetProbabilityBJetTags', 'pfCombinedMVABJetTags',
            'pfSimpleSecondaryVertexHighEffBJetTags', 'pfSimpleSecondaryVertexHighPurBJetTags'
            ]

    if redoJEC:
        setup_jets_(process, isData, bTagDiscriminators)
        setup_met_(process, isData)

    # Producers
    from cp3_llbb.Framework import EventProducer
    from cp3_llbb.Framework import GenParticlesProducer
    from cp3_llbb.Framework import HLTProducer
    from cp3_llbb.Framework import JetsProducer
    from cp3_llbb.Framework import METProducer
    from cp3_llbb.Framework import MuonsProducer
    from cp3_llbb.Framework import ElectronsProducer
    from cp3_llbb.Framework import VerticesProducer

    output = 'output_mc.root' if not isData else 'output_data.root'

    process.framework = cms.EDProducer("ExTreeMaker",
            output = cms.string(output),

            filters = cms.PSet(
                ),

            producers = cms.PSet(

                event = EventProducer.default_configuration,

                hlt = HLTProducer.default_configuration,

                jets = JetsProducer.default_configuration,

                met = METProducer.default_configuration,

                muons = MuonsProducer.default_configuration,

                electrons = ElectronsProducer.default_configuration,

                vertices = VerticesProducer.default_configuration,
                ),

            analyzers = analyzers
            )

    process.framework.producers.jets.parameters.cut = cms.untracked.string("pt > 10")
    process.framework.producers.jets.parameters.btags = cms.untracked.vstring(bTagDiscriminators)

    path = cms.Path()

    if not isData:
        process.framework.producers.gen_particles = GenParticlesProducer.default_configuration
    else:
        # MET Filters

        from cp3_llbb.Framework import METFilter
        process.framework.filters.met = METFilter.default_configuration

        process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
        process.filterOnHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
           inputLabel = cms.InputTag('HBHENoiseFilterResultProducer', 'HBHENoiseFilterResult'),
           reverseDecision = cms.bool(False)
        )

        path += cms.Sequence(process.filterOnHBHENoiseFilter) 

    if redoJEC:
        process.framework.producers.jets.parameters.jets = cms.untracked.InputTag('selectedPatJetsAK4PFCHS')

    path += cms.Sequence(
            process.egmGsfElectronIDSequence *
            process.framework
            )

    process.p = path

    #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    #! Output and Log
    #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    return process

def schedule(process, analyzers):
    """Inform the framework of the desired scheduling of the analyzers

    Args:
        process (cms.Process): the current framework process, return by the ``create`` method
        analyzers (string tuple): a string array representing the desired scheduling of the analyzers. Analyzers will be executed in the specified order.
    """

    if analyzers is None or len(analyzers) == 0:
        return process

    framework_analyzers = process.framework.analyzers.parameterNames_()
    for a in framework_analyzers:
        if not a in analyzers:
            raise Exception('Analyzer %r not found in your scheduling array. Please add it somewhere suitable for you.' % a)

    if len(analyzers) != len(framework_analyzers):
        raise Exception('Your scheduling array contains a different number of analyzers than the framework (%d vs %d)' % (len(analyzers), len(framework_analyzers)))

    process.framework.analyzers_scheduling = cms.untracked.vstring(analyzers)

    return process
