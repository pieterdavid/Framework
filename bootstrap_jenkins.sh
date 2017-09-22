#! /bin/sh

# CMSSW env is already configured
# Current working dir is $CMSSW_BASE/src

git cms-init

git remote add cms-egamma https://github.com/cms-egamma/cmssw.git
git fetch cms-egamma EGM_gain_v1
git cms-merge-topic cms-egamma:f2ae5ef247d2544bdccd0460586b468eb35631c5
git remote add rafaellopesdesa https://github.com/rafaellopesdesa/cmssw.git
git fetch rafaellopesdesa RegressionCheckNegEnergy
git cms-merge-topic rafaellopesdesa:3aafeff0371a1d1eb3db9d95ef50c1a66da25690
git remote add cms-met https://github.com/cms-met/cmssw.git
git fetch cms-met METRecipe_8020_for80Xintegration
git cms-merge-topic cms-met:92f73cd3d16a9529585865a365de271e0535b68d

git clone -o upstream https://github.com/bachtis/analysis.git -b KaMuCa_V4 KaMuCa 
pushd KaMuCa
git checkout 2ad38daae37a41a9c07f482e95f2455e3eb915b0
popd
git clone -o upstream https://github.com/blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
