from FWCore.ParameterSet.VarParsing import VarParsing

class CmdLine(object):
    """
    Parse the command line for options. Supported options are:
        - 'globalTag': the global tag to use
        - 'era': '25ns' or '50ns' (for 2015 data) or '2016'
        - 'process': The process name used in the MiniAOD generation
        - 'runOnData': 1 if running on data, 0 otherwise
        - 'hltProcessName': the process name used when running the HLT

    Used mainly by GridIn and crab to override user configuration of the framework with correct values
    for a given dataset.
    """

    def __init__(self, required=[]):
        options = VarParsing()

        options.register('runOnData',
                -1,
                VarParsing.multiplicity.singleton,
                VarParsing.varType.int,
                'If running over MC (0) or data (1)')

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

        options.register('process',
                '',
                VarParsing.multiplicity.singleton,
                VarParsing.varType.string,
                'Process name of the MiniAOD production.')

        options.register('hltProcessName',
                '',
                VarParsing.multiplicity.singleton,
                VarParsing.varType.string,
                'The HLT processName to use')

        options.parseArguments()

        # Sanity checks
        if options.era:
            assert options.era == '25ns' or options.era == '50ns' or options.era == '2016'

        self.options = options

    def __getattr__(self, name):
        """
        Forward call to the options object
        """

        return getattr(self.options, name)
