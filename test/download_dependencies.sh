#!/bin/bash

[ -f Spring16_25nsV1_MC.db ] || wget https://github.com/cms-jet/JECDatabase/raw/master/SQLiteFiles/Spring16_25nsV1_MC.db

# Input files

if [ -d /afs/cern.ch/user/o/obondu/public/CP3 ]; then
    ln -s /afs/cern.ch/user/o/obondu/public/CP3/*.root .
else
    [ -f DoubleMuon_Run2016B_PromptReco-v2_reduced.root ] || wget https://obondu.web.cern.ch/obondu/public/CP3/DoubleMuon_Run2016B_PromptReco-v2_reduced.root
    [ -f TTTo2L2Nu_13TeV-powheg_RunIISpring16MiniAODv2_reduced.root ] || wget https://obondu.web.cern.ch/obondu/public/CP3/TTTo2L2Nu_13TeV-powheg_RunIISpring16MiniAODv2_reduced.root
fi
