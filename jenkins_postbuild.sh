#! /bin/sh

# This script is executed after `scram b`
cd ${CMSSW_BASE}/src/EgammaAnalysis/ElectronTools/data
git clone https://github.com/ECALELFS/ScalesSmearings.git
cd ScalesSmearings
git checkout Moriond17_23Jan_v1
cd ${CMSSW_BASE}/src
