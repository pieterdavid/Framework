#! /bin/sh

# CMSSW env is already configured
# Current working dir is $CMSSW_BASE/src

git cms-merge-topic rafaellopesdesa:EgammaAnalysis80_EGMSmearer_Moriond17_23Jan
git cms-merge-topic rafaellopesdesa:RegressionCheckNegEnergy
git remote add cms-met https://github.com/cms-met/cmssw
git fetch cms-met METRecipe_8020_for80Xintegration
git cms-merge-topic cms-met:92f73cd3d16a9529585865a365de271e0535b68d -u

git clone -o upstream https://github.com/bachtis/analysis.git -b KaMuCa_V4 KaMuCa 
pushd KaMuCa
git checkout 2ad38daae37a41a9c07f482e95f2455e3eb915b0
popd
git clone -o upstream https://github.com/blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
