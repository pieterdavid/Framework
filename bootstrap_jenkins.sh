#! /bin/sh

# CMSSW env is already configured
# Current working dir is $CMSSW_BASE/src

git cms-merge-topic ikrav:egm_id_747_v2
git clone https://github.com/cms-jet/JetToolbox JMEAnalysis/JetToolbox
git cms-merge-topic 11007
git cms-merge-topic -u cms-met:METCorUnc74X
git clone -o upstream https://github.com/blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
