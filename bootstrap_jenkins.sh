#! /bin/sh

# CMSSW env is already configured
# Current working dir is $CMSSW_BASE/src

git cms-merge-topic ikrav:egm_id_80X_v2
git cms-merge-topic rafaellopesdesa:Regression80XEgammaAnalysis_v2
git cms-merge-topic shervin86:Moriond2017_JEC_energyScales
git cms-merge-topic gpetruc:badMuonFilters_80X_v2

git remote add rafaellopesdesa https://github.com/rafaellopesdesa/cmssw.git
git fetch rafaellopesdesa RegressionCheckNegEnergy
git cherry-pick 67da8f10d8fa125197734ccea701035dd1020bd7
git cherry-pick 3aafeff0371a1d1eb3db9d95ef50c1a66da25690
git remote remove rafaellopesdesa

git clone -o upstream https://github.com/bachtis/analysis.git -b KaMuCa_V4 KaMuCa 
pushd KaMuCa
git checkout 2ad38daae37a41a9c07f482e95f2455e3eb915b0
popd
git clone -o upstream https://github.com/blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
