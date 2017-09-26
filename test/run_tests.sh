#!/bin/bash

first_failure_status=0
first_failure_message=""
# Pass in name and status
function die {
  first_failure_message=$1
  first_failure_status=$2
  echo "${first_failure_message}: status ${first_failure_status}"
}

. download_dependencies.sh

F1=${LOCAL_TEST_DIR}/unit_tests_mc.py
(cmsRun $F1 ) || die "Failure using $F1" $?
(cp3llbbTestFrameworkOutput ${LOCAL_TEST_DIR}/unit_tests_mc_ref.root output_mc.root) || die "Output file does not match reference file" $?

F2=${LOCAL_TEST_DIR}/unit_tests_data.py
(cmsRun $F2 ) || die "Failure using $F2" $?
(cp3llbbTestFrameworkOutput ${LOCAL_TEST_DIR}/unit_tests_data_ref.root output_data.root) || die "Output file does not match reference file" $?

F3=${LOCAL_TEST_DIR}/unit_tests_mc_with_db.py
(cmsRun $F3 ) || die "Failure using $F3" $?
(cp3llbbTestFrameworkOutput ${LOCAL_TEST_DIR}/unit_tests_mc_with_db_ref.root output_mc.root) || die "Output file does not match reference file" $?

if [ ${first_failure_status} -ne 0 ]; then
  exit $first_failure_status
fi
