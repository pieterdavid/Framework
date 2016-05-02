#! /bin/bash

wget https://github.com/cms-jet/JECDatabase/raw/master/SQLiteFiles/Spring16_25nsV1_MC.db
(cmsRun unit_tests_mc.py) && mv output_mc.root unit_tests_mc_ref.root
(cmsRun unit_tests_mc_with_db.py) && mv output_mc.root unit_tests_mc_with_db_ref.root
(cmsRun unit_tests_data.py) && mv output_data.root unit_tests_data_ref.root
