#! /bin/sh

startdir=$(pwd)

cmssw_arch="slc6_amd64_gcc530" ## should be in sync with CMSSW.arch
cmssw_version="CMSSW_8_0_30"   ## should be in sync with CMSSW.release

scramv1 -a "${cmssw_arch}" project CMSSW "${cmssw_version}"
cd "${cmssw_version}"
cmsenv

git clone -o upstream https://github.com/cp3-llbb/Framework
fwkdir_tmp="${CMSSW_BASE}/Framework"

upstream_cmssw_arch=$(cat "Framework/CMSSW.arch")
if [ "${upstream_cmssw_arch}" != "${cmssw_arch}" ]; then
  echo "Platform mismatch with CMSSW.arch: ${upstream_cmssw_arch}, while set up with ${cmssw_arch}"
  exit 1
fi
upstream_cmssw_version=$(cat "Framework/CMSSW.release")
if [ "${upstream_cmssw_version}" != "${cmssw_version}" ]; then
  echo "CMSSW version mismatch with CMSSW.release: ${upstream_cmssw_version}, while set up with ${cmssw_version}"
  exit 1
fi

## apply recipes
cd "${CMSSW_BASE}/src"
. "${fwkdir_tmp}/bootstrap_jenkins.sh"
## move Framework to the right place
llbbdir="${CMSSW_BASE}/src/cp3_llbb"
mkdir -p "${llbbdir}"
mv "${fwkdir_tmp}" "${llbbdir}/"
## compile
scram b -j4
## check out a few more
. "${fwkpkg}/jenkins_postbuild.sh"

## add git remotes
cd "${llbbdir}/Framework"
. ./first_setup.sh

cd "${startdir}"

echo "================================================================================"
echo "Set up project area for CMSSW version ${cmssw_version} in ${CMSSW_BASE}"
echo "Next, cd ${CMSSW_BASE}/src; cmsenv, clone your analysis package, scram b, etc."
echo "================================================================================"
