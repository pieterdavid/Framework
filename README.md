# Framework
Common framework for all cp3-llbb analyses

*Please note:*
* The instructions are for the UCLouvain ingrid SLC6 cluster (to access SAMADhi)
* You need the proper username and password to access SAMADhi :) If you don't know what this is about, ask around
* The current state of the art mini-AOD documentation can be found [here](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015)
* You will probably want to install as well [GridIn](https://github.com/cp3-llbb/GridIn) to run jobs on the grid, and one of the existing analyses ([TTAnalysis](https://github.com/cp3-llbb/TTAnalysis), [HHAnalysis](https://github.com/cp3-llbb/HHAnalysis))



## First time setup instructions

```bash
source /nfs/soft/grid/ui_sl6/setup/grid-env.sh
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_4_10
cd CMSSW_7_4_10/src
cmsenv

git cms-init
cd ${CMSSW_BASE}/src 

# Electron ID as from [EGamma twiki (as on September 1st 2015)](https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2)
git cms-merge-topic ikrav:egm_id_747_v2

# Jet tool box
git clone https://github.com/cms-jet/JetToolbox JMEAnalysis/JetToolbox

# Backport of new pileup jet id. Not needed for CMSSW 7.4.12+
git cms-merge-topic 11007

# MET stuff
git cms-merge-topic -u cms-met:METCorUnc74X

# CP3-llbb framework itself
git clone -o upstream git@github.com:blinkseb/TreeWrapper.git cp3_llbb/TreeWrapper
git clone -o upstream git@github.com:cp3-llbb/Framework.git cp3_llbb/Framework

cd ${CMSSW_BASE}/src
scram b -j 4

cd ${CMSSW_BASE}/src/cp3_llbb/Framework
source setup.sh
cd ${CMSSW_BASE}/src
```

If you are using ingrid, here's a useful alias to put in your ``bashrc`` file:

```bash
alias cms_env="module purge; module load grid/grid_environment_sl6; module load crab/crab3; module load cms/cmssw;"
```

Then, just do ``cms_env`` to load all the CMSSW environment.

## Test run (command line)

```bash
cd ${CMSSW_BASE}/src/cp3_llbb/Framework/test
cmsRun TestConfigurationMC.py
```

# When willing to commit things
  * Remember to *branch before committing anything*: ```git checkout -b my-new-branch```
  * The ```setup.sh``` script took care of adding ```origin``` as your own repo, so to push just do the usual ```git push origin my-new-branch```
