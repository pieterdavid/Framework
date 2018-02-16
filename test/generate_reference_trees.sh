#! /bin/bash

. download_dependencies.sh

(cmsRun unit_tests_mc.py) && mv output_mc.root newref_mc.root
status_mc=$?
(cmsRun unit_tests_mc_with_db.py) && mv output_mc.root newref_mc_with_db.root
status_mc_with_db=$?
(cmsRun unit_tests_data.py) && mv output_data.root newref_data.root
status_data=$?

function update_ref_file() {
  local cmsRun_status=$1
  local suffix=$2
  local outfile="newref_${suffix}.root"
  local target="unit_tests_${suffix}_ref.root"
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
