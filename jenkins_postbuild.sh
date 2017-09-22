#! /bin/sh

# This script is executed after `scram b`
cd ${CMSSW_BASE}/src/EgammaAnalysis/ElectronTools/data
git clone https://github.com/ECALELFS/ScalesSmearings.git
cd ScalesSmearings
git checkout fe4ce4355ef88e4fc0efaa6bad06c25e333fdb86 # corresponds to HEAD of branch Moriond17_gainSwitch_unc
cd ${CMSSW_BASE}/src
