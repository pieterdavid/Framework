# Framework
Common framework for all cp3-llbb analyses

## Setup instructions

```bash

export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_4_4
cd CMSSW_7_4_4/src
cmsenv

git cms-init
cd ${CMSSW_BASE}/src 

git clone -o upstream git@github.com:blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
git clone -o upstream git@github.com:cp3-llbb/Framework.git cp3_llbb/Framework

cd cp3_llbb/Framework

cd ${CMSSW_BASE}/src
scram b -j 4

cd cp3_llbb/Framework/test

cmsRun TestConfiguration.py


```
