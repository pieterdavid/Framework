# Framework
Common framework for all cp3-llbb analyses

*Please note:*
* The instructions are for the UCLouvain ingrid SLC6 cluster (to access SAMADhi)
* You need the proper username and password to access SAMADhi :) If you don't know what this is about, ask around
* The current state of the art mini-AOD documentation can be found [here](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015)



## First time setup instructions

```bash
source /nfs/soft/grid/ui_sl6/setup/grid-env.sh
source /cvmfs/cms.cern.ch/cmsset_default.sh
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
```

## Test run (command line)

```bash
cd ${CMSSW_BASE}/src/cp3_llbb/Framework/test
cmsRun TestConfiguration.py
```

# When willing to commit things
  * Remember to *branch before committing anything*: ```git checkout -b my-new-branch```
  * The ```setup.sh``` script took care of adding ```origin``` as your own repo, so to push just do the usual ```git push origin my-new-branch```

