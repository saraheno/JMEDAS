JMEDAS
======

CMS Data Analysis School (CMSDAS) exercise



`cmsrel CMSSW_8_0_12`

`cd CMSSW_8_0_12/src`

`git clone  https://github.com/kovalch/JMEDAS.git Analysis/JMEDAS`

`cd Analysis/JMEDAS`

`scram b -j 10`

`cd test`

`voms-proxy-init`

`python jmedas_fwlite.py --files=ttjets_short.txt --outname=ttjets_short.root --maxevents=2000 --maxjets=2`

