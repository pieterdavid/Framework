import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

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
    if not isData:
        process.genMetExtractor = cms.EDProducer("GenMETExtractor",
                metSource = cms.InputTag("slimmedMETs", "" , cms.InputTag.skipCurrentProcess())
                )

    # MET is done from all PF candidates, and Type-I corrections are computed from CHS ak4 PF jets
    # https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#type_1_PF_MET_recommended

    ## Raw PF METs
    process.load('RecoMET.METProducers.PFMET_cfi')

    process.pfMet.src = cms.InputTag('packedPFCandidates')
    addMETCollection(process, labelName='patPFMet', metSource='pfMet') # RAW MET
    process.patPFMet.addGenMET = False

    ## Type 1 corrections
    process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
    from JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff import corrPfMetType1
    from JetMETCorrections.Type1MET.correctedMet_cff import pfMetT1

    if not hasattr(process, 'ak4PFJetsCHS'):
        print("WARNING: No AK4 CHS jets produced. Type 1 corrections for MET are not available.")
    else:
        process.corrPfMetType1 = corrPfMetType1.clone(
            src = 'ak4PFJetsCHS',
            jetCorrLabel = 'ak4PFCHSL1FastL2L3Corrector' if not isData else 'ak4PFCHSL1FastL2L3ResidualCorrector',
            offsetCorrLabel = 'ak4PFCHSL1FastjetCorrector'
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
        if not isData:
            process.patMET.genMETSource = cms.InputTag("genMetExtractor")
        process.slimmedMETs.src = cms.InputTag("patMET")
        process.slimmedMETs.rawUncertainties = cms.InputTag("patPFMet") # only central value
    else:
        # Create MET from RAW PF collection
        process.patPFMet.addGenMET = not isData
        if not isData:
            process.patPFMet.genMETSource = cms.InputTag("genMetExtractor")
        process.slimmedMETs.src = cms.InputTag("patPFMet")
        del process.slimmedMETs.rawUncertainties # not available

    del process.slimmedMETs.type1Uncertainties # not available
    del process.slimmedMETs.type1p2Uncertainties # not available

def setup_nohf_met_(process, isData):
    from PhysicsTools.PatAlgos.tools.metTools import addMETCollection

    process.noHFCands = cms.EDFilter("CandPtrSelector",
            src=cms.InputTag("packedPFCandidates"),
            cut=cms.string("abs(pdgId)!=1 && abs(pdgId)!=2 && abs(eta)<3.0")
            )

    if not isData and not hasattr(process, 'genMetExtractor'):
        process.genMetExtractor = cms.EDProducer("GenMETExtractor",
                metSource = cms.InputTag("slimmedMETs", "" , cms.InputTag.skipCurrentProcess())
                )

    if not hasattr(process, 'pfMet'):
        from RecoMET.METProducers.PFMET_cfi import pfMet
        process.pfMetNoHF = pfMet.clone()
    else:
        process.pfMetNoHF = process.pfMet.clone()

    process.pfMetNoHF.calculateSignificance = False
    process.pfMetNoHF.src = cms.InputTag('noHFCands')
    addMETCollection(process, labelName='patPFMetNoHF', metSource='pfMetNoHF') # RAW MET
    process.patPFMetNoHF.addGenMET = False

    ## Type 1 corrections
    if not hasattr(process, 'ak4PFCHSL1FastL2L3Corrector'):
        process.load('JetMETCorrections.Configuration.JetCorrectors_cff')

    from JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff import corrPfMetType1
    from JetMETCorrections.Type1MET.correctedMet_cff import pfMetT1

    if not hasattr(process, 'ak4PFJetsCHS'):
        print("WARNING: No AK4 CHS jets produced. Type 1 corrections for MET are not available.")
    else:
        if not hasattr(process, 'corrPfMetType1'):
            process.corrPfMetType1 = corrPfMetType1.clone(
                src = 'ak4PFJetsCHS',
                jetCorrLabel = 'ak4PFCHSL1FastL2L3Corrector' if not isData else 'ak4PFCHSL1FastL2L3ResidualCorrector',
                offsetCorrLabel = 'ak4PFCHSL1FastjetCorrector'
            )

        process.pfMetT1NoHF = pfMetT1.clone(
            src = 'pfMetNoHF',
            srcCorrections = [cms.InputTag("corrPfMetType1", "type1")]
        )

        addMETCollection(process, labelName='patMETNoHF', metSource='pfMetT1NoHF') # T1 MET
        process.patMETNoHF.addGenMET = False

    ## Slimmed METs

    from PhysicsTools.PatAlgos.slimming.slimmedMETs_cfi import slimmedMETs
    #### CaloMET is not available in MiniAOD
    if hasattr(slimmedMETs, 'caloMET'):
        del slimmedMETs.caloMET

    process.slimmedMETsNoHF = slimmedMETs.clone()
    if hasattr(process, "patMETNoHF"):
        # Create MET from Type 1 PF collection
        process.patMETNoHF.addGenMET = not isData
        if not isData:
            process.patMETNoHF.genMETSource = cms.InputTag("genMetExtractor")
        process.slimmedMETsNoHF.src = cms.InputTag("patMETNoHF")
        process.slimmedMETsNoHF.rawUncertainties = cms.InputTag("patPFMetNoHF") # only central value
    else:
        # Create MET from RAW PF collection
        process.patPFMetNoHF.addGenMET = not isData
        if not isData:
            process.patPFMetNoHF.genMETSource = cms.InputTag("genMetExtractor")
        process.slimmedMETsNoHF.src = cms.InputTag("patPFMetNoHF")
        del process.slimmedMETsNoHF.rawUncertainties # not available

    del process.slimmedMETsNoHF.type1Uncertainties # not available
    del process.slimmedMETsNoHF.type1p2Uncertainties # not available


def create(isData, era, globalTag=None, analyzers=cms.PSet(), redoJEC=False):
    """Create the CMSSW python configuration for the Framework

    Args:
        isData (bool): Set to True if you run over data, or False for simulated samples
        era (Configuration.StandardSequences.Eras.eras): The era of the sample. Used to distringuish between 50ns and 25ns
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
        process = Framework.create(True, eras.Run2_50ns, '74X_dataRun2_Prompt_v1')

        # Run on 25ns data sample
        process = Framework.create(True, eras.Run2_25ns, '74X_dataRun2_Prompt_v2')

        # For MC 50ns
        process = Framework.create(False, eras.Run2_50ns, 'MCRUN2_74_V9', cms.PSet(
                test = cms.PSet(
                    type = cms.string('test_analyzer'),
                    prefix = cms.string('test_'),
                    enable = cms.bool(True)
                    )
                )
            )
    """

    parseCommandLine = False
    import sys
    for argv in sys.argv:
        if 'globalTag' in argv or 'era' in argv:
            parseCommandLine = True
            break

    if parseCommandLine:
        from FWCore.ParameterSet.VarParsing import VarParsing
        options = VarParsing()
        options.register('globalTag',
                '',
                VarParsing.multiplicity.singleton,
            VarParsing.varType.string,
                'The globaltag to use')

        options.register('era',
                '',
                VarParsing.multiplicity.singleton,
                VarParsing.varType.string,
                'Era of the dataset')

        options.parseArguments()

        if options.globalTag:
            globalTag = options.globalTag

        if options.era:
            assert options.era == '25ns' or options.era == '50ns'
            if options.era == '25ns':
                era = eras.Run2_25ns
            else:
                era = eras.Run2_50ns



    if globalTag is None:
        raise Exception("No global tag specified. Use the 'globalTag' command line argument to set one.\n\tcmsRun <configuration> globalTag=<global_tag>")

    if era is None:
        raise Exception("No era specified. Use the 'era' command line argument to set one.\n\tcmsRun <configuration> era=[25ns|50ns]")

    print("")
    print("CP3 llbb framework:")
    print("\t - Running over %s" % ("data" if isData else "simulation"))
    print("\t - global tag: %s" % globalTag)
    print("\t - era: %s" % ("25ns" if era == eras.Run2_25ns else "50ns"))
    print("")

    process = cms.Process("ETM", era)

    process.options = cms.untracked.PSet(
            wantSummary = cms.untracked.bool(False),
            allowUnscheduled = cms.untracked.bool(True)
            )

    # Flags
    createNoHFMet = True

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

    if createNoHFMet:
        setup_nohf_met_(process, isData)

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

    if era == eras.Run2_25ns:
        process.framework.producers.electrons.parameters.ea_R03 = cms.untracked.FileInPath('RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt')
    else:
        process.framework.producers.electrons.parameters.ea_R03 = cms.untracked.FileInPath('RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_50ns.txt')

    if createNoHFMet:
        process.framework.producers.nohf_met = cms.PSet(METProducer.default_configuration.clone())
        process.framework.producers.nohf_met.prefix = 'nohf_met_'
        process.framework.producers.nohf_met.parameters.met = cms.untracked.InputTag('slimmedMETsNoHF')

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
