#! /bin/sh

startdir=$(pwd)

tempdir=$(mktemp -d)
fwkdir_tmp="${tempdir}/Framework"

git clone -o upstream https://github.com/cp3-llbb/Framework "${fwkdir_tmp}"

cmssw_arch=$(cat "${fwkdir_tmp}/CMSSW.arch")
cmssw_version=$(cat "${fwkdir_tmp}/CMSSW.release")

scramv1 -a "${cmssw_arch}" project CMSSW "${cmssw_version}"
cd "${cmssw_version}"
cmsenv

## apply recipes
cd "${CMSSW_BASE}/src"
. "${fwkdir_tmp}/bootstrap_jenkins.sh"
## move Framework to the right place
llbbdir="${CMSSW_BASE}/src/cp3_llbb"
mkdir -p "${llbbdir}"
mv "${fwkdir_tmp}" "${llbbdir}/"
rmdir "${tempdir}"
## compile
scram b -j7
## check out a few more
. "${llbbdir}/Framework/jenkins_postbuild.sh"

## add git remotes
cd "${llbbdir}/Framework"
. ./first_setup.sh

cd "${startdir}"

echo "================================================================================"
echo "Set up project area for CMSSW version ${cmssw_version} in ${CMSSW_BASE}"
echo "Next, cd ${CMSSW_BASE}/src; cmsenv, clone your analysis package, scram b, etc."
echo "================================================================================"
