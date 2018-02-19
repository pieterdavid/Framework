#! /bin/bash

check_git=true
check_scram=true
ncores=3
## parse options
tmp_opts=$(getopt --longoptions=skipgitcheck,skipscram,help --options=j:,h -- $@)
eval set -- "${tmp_opts}"
while true; do
  case "$1" in
    --skipgitcheck)
      check_git=false
      shift ;;
    --skipscram)
      check_scram=false
      shift ;;
    -j)
      ncores="$2"
      shift 2 ;;
    -h|--help)
      echo "Usage: $0 -jN"
      echo ""
      echo "By default, this script will also check that the git history of CMSSW and the Framework are clean."
      echo "These checks can be overridden with the flags --skipgitcheck and --skipscram."
      exit 0 ;;
    --)
      shift; break ;;
  esac
done

## check env and paths
cmsswdir="${CMSSW_BASE}/src"
fwkdir="${cmsswdir}/cp3_llbb/Framework"
testdir="${fwkdir}/test"
if [[ -n "${cmsswdir}" ]]; then
  if [[ ! -d "${cmsswdir}" ]]; then
    echo "CMSSW_BASE/src (${cmsswdir}) is not a directory, please check your environment setup (e.g. run cmsenv)"
    exit 1
  else ## also check Framework
    if [[ ! -d "${cmsswdir}/.git" ]]; then
      echo "Git directory for CMSSW (${cmsswdir}/.git) does not exist, please run git cms-init"
      exit 1
    fi
    if [[ ! -d "${fwkdir}" ]]; then
      echo "cp3-llbb Framework path (${fwkdir}) is not a directory!"
      exit 1
    else
      if [[ ! -d "${fwkdir}/.git" ]]; then
        echo "Git directory for CMSSW (${fwkdir}/.git) does not exist, cannot verify if it is clean then"
        exit 1
      fi
    fi
  fi
else
  echo "CMSSW_BASE/src is not defined, please check your environment setup (e.g. run cmsenv)"
  exit 1
fi

## First step: check that the repository is clean and compiled
if [[ "${check_git}" = true ]]; then
  echo "---> Checking that everything is committed"
  pushd "${cmsswdir}" > /dev/null
  out_cmssw=$(git status --porcelain)
  if [[ $? != 0 ]]; then
    echo "git status failed in ${cmsswdir}"; popd > /dev/null; exit 1
  fi
  popd > /dev/null
  out_cmssw=$(echo "${out_cmssw}" | grep -v "^?? KaMuCa/$" | grep -v "^?? cp3_llbb/$" | grep -v "^?? EgammaAnalysis/ElectronTools/data/ScalesSmearings/$")
  if [[ -n "${out_cmssw}" ]]; then
    echo "CMSSW project area is not clean, aborting"; echo "${out_cmssw}"; exit 1
  fi
  echo "CMSSW OK"
  pushd "${fwkdir}" > /dev/null
  out_fwk=$(git status --porcelain)
  if [[ $? != 0 ]]; then
    echo "git status failed in ${fwkdir}"; popd > /dev/null; exit 1
  fi
  popd > /dev/null
  if [[ -n "${out_fwk}" ]]; then
    echo "Framework package is not clean, aborting"; echo "${out_fwk}"; exit 1
  fi
  echo "Framework OK"
fi
if [[ "${check_scram}" = true ]]; then
  echo "---> Compiling in ${cmsswdir} with scram b -j${ncores}"
  pushd "${cmsswdir}" > /dev/null
  scram b -j${ncores}
  scram_status=$?
  popd > /dev/null
  if [[ "${scram_status}" != 0 ]]; then
    echo "---> scram failed with status ${scram_status}"
    exit ${scram_status}
  else
    echo "---> Successfully compiled"
  fi
fi

## Run the tests
${testdir}/download_dependencies.sh

echo "---> Running unit_tests_mc.py"
(cmsRun "${testdir}/unit_tests_mc.py") && mv output_mc.root newref_mc.root
status_mc=$?
echo "---> Running unit_tests_mc_with_db.py"
(cmsRun "${testdir}/unit_tests_mc_with_db.py") && mv output_mc.root newref_mc_with_db.root
status_mc_with_db=$?
echo "---> Running unit_tests_data.py"
(cmsRun "${testdir}/unit_tests_data.py") && mv output_data.root newref_data.root
status_data=$?

## Change the references
function update_ref_file() {
  local cmsRun_status=$1
  local suffix=$2
  local outfile="newref_${suffix}.root"
  local target="${fwkdir}/test/unit_tests_${suffix}_ref.root"
  if [[ ${cmsRun_status} -eq 0 ]]; then
    echo "---> Checking output for unit_tests_${suffix}..."
    if (cp3llbbTestFrameworkOutput "${target}" "${outfile}"); then
      echo "---> No differences, saved output as ${outfile}"
    else
      mv "${outfile}" "${target}"
      echo "---> Updated ${target}"
    fi
  fi
}

update_ref_file ${status_mc} "mc"
update_ref_file ${status_mc_with_db} "mc_with_db"
update_ref_file ${status_data} "data"
