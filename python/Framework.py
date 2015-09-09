import FWCore.ParameterSet.Config as cms

def create(era, globalTag=None, analyzers=cms.PSet()):
    """Create the CMSSW python configuration for the Framework

    Args:
        era (Configuration.StandardSequences.Eras.eras): The era of the data sample. Use ``None`` for simulated samples
        globalTag (str): The global tag to use for this workflow. If set to ``None``, a command-line argument named ``globalTag`` must be specified
        analyzers (cms.PSet()): A list of analyzers to run. By default, it's empty, and you can still add one to the list afterwards.

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
