#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

F1=${LOCAL_TEST_DIR}/unit_tests_mc.py
(cmsRun $F1 ) || die "Failure using $F1" $?

F2=${LOCAL_TEST_DIR}/unit_tests_data.py
(cmsRun $F2 ) || die "Failure using $F2" $?

wget https://github.com/cms-jet/JECDatabase/raw/master/SQLiteFiles/Summer15_25nsV5_MC.db
F3=${LOCAL_TEST_DIR}/unit_tests_mc_with_db.py
(cmsRun $F3 ) || die "Failure using $F3" $?
