TrackingNtuple

how-to sparse-checkout only this directory

git init TrackingTests <br>
cd TrackingTests/ <br>
git remote add -f origin git@github.com:cerati/tracking-tests  <br>
git config core.sparseCheckout true  <br>
echo "TrackingNtuple/" >> .git/info/sparse-checkout  <br>
git pull origin master  <br>
cd TrackingNtuple/  <br>

Working setup
* CMSSW_7_4_14
* Instructions above to install TrackingNtuple
* In local area I have also TrackingTools/(MaterialEffects,GeomPropagators,KalmanUpdators) and RecoTracker/TkNavigation, but I think these are not strictly needed
* Create samples up to step2 using configuration files in directories, e.g. SingleMuPt10 (these originally come from runTheMatrix)
* Edit python/ConfFile_cfg.py and make ntuples running 'cmsRun python/ConfFile_cfg.py' (note that lines below 'mock' and 'noSplit' activate special options, i.e. to bring initialStep closer to what we do and to avoid splitting of matched hits, respectively)
* In WriteMemoryFile.cc, edit input and output file names, number of events (note that it crashes if you are too optimistic in the number of saved events), set flags (e.g. useMatched, doEndcap)
* Compile with make, note that it needs stuff from mictest, assumed to be in MICTESTDIR (edit Makefile accordingly)
* Get memory dump with ./writeMemoryFile