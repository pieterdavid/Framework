# Framework
Common framework for all cp3-llbb analyses

## Setup instructions

```bash

export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_4_5
cd CMSSW_7_4_5/src
cmsenv

git cms-init
cd ${CMSSW_BASE}/src 

# Electron ID as from [June 28th 2015 EGamma hypernews](https://hypernews.cern.ch/HyperNews/CMS/get/egamma/1589.html)
git cms-merge-topic ikrav:egm_id_74X_v2

# CP3-llbb framework itself
git clone -o upstream git@github.com:blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
git clone -o upstream git@github.com:cp3-llbb/Framework.git cp3_llbb/Framework

cd ${CMSSW_BASE}/src
scram b -j 4

cd cp3_llbb/Framework/test

cmsRun TestConfiguration.py


```
