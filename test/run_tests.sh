#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

F1=${LOCAL_TEST_DIR}/unit_tests_mc.py
(cmsRun $F1 ) || die "Failure using $F1" $?
(cp3llbbTestFrameworkOutput ${LOCAL_TEST_DIR}/unit_tests_mc_ref.root output_mc.root) || die "Output file does not match reference file" $?

F2=${LOCAL_TEST_DIR}/unit_tests_data.py
(cmsRun $F2 ) || die "Failure using $F2" $?
(cp3llbbTestFrameworkOutput ${LOCAL_TEST_DIR}/unit_tests_data_ref.root output_data.root) || die "Output file does not match reference file" $?

# FIXME:
# Test failing in CMSSW 7.6.3 because of a CMSSW bug
# See https://hypernews.cern.ch/HyperNews/CMS/get/calibrations/2196/1.html
# Re-enable with CMSSW 7.6.4+
# wget https://github.com/cms-jet/JECDatabase/raw/master/SQLiteFiles/Summer15_25nsV5_MC.db
# F3=${LOCAL_TEST_DIR}/unit_tests_mc_with_db.py
# (cmsRun $F3 ) || die "Failure using $F3" $?
# (cp3llbbTestFrameworkOutput ${LOCAL_TEST_DIR}/unit_tests_mc_with_db_ref.root output_mc.root) || die "Output file does not match reference file" $?
