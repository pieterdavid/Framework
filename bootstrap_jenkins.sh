#! /bin/sh

# CMSSW env is already configured
# Current working dir is $CMSSW_BASE/src

git cms-addpkg DataFormats/PatCandidates
git remote add blinkseb https://github.com/blinkseb/cmssw.git
git fetch blinkseb
git cherry-pick 794364a7352f045d7eed1fcc166fb92ece8ce68f
git clone https://github.com/cms-jet/JetToolbox JMEAnalysis/JetToolbox
git clone -o upstream https://github.com/blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
