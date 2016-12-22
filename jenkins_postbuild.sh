#! /bin/sh

# This script is executed after `scram b`

cd ${CMSSW_BASE}/external/${SCRAM_ARCH}
git clone https://github.com/ikrav/RecoEgamma-ElectronIdentification.git data/RecoEgamma/ElectronIdentification/data
cd data/RecoEgamma/ElectronIdentification/data
git checkout egm_id_80X_v1
cd ${CMSSW_BASE}/src/EgammaAnalysis/ElectronTools/data
git clone https://github.com/ECALELFS/ScalesSmearings.git
cd ${CMSSW_BASE}/src
