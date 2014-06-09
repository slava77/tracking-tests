#!/bin/bash


pileup=(PU25_BX50 PU50_BX50 PU25_BX25 PU40_BX25 PU70_BX25)

files=(53X 62X 710pre7-dyn-ineff 710pre7-no-dyn-ineff 710pre8-dyn-ineff 710pre8-dyn-ineff-digi-pre7 710pre8-no-dyn-ineff)
colors=(kGreen-1 kOrange-1 kViolet+2 kViolet kAzure+1 kAzure-1 kAzure)
markers=(kFullCircle kFullCircle kOpenCircle kFullCircle kOpenCircle kOpenCircle kFullCircle)

#these are the positions to compare in the array above 
comp1=(0 6 3 0 4 1)
comp2=(6 4 6 1 5 6)

for p in ${pileup[@]} ; do 
    count=0
    for f in ${comp1[@]} ; do 
	root -b -q 'TrackValHistoPublisher.C("plots_'$p'_'${files[${comp2[count]}]}'_vs_'${files[${comp1[count]}]}'","multitrackValidator_'$p'_'${files[${comp2[count]}]}'.root","cutsReco","multitrackValidator_'$p'_'${files[${comp1[count]}]}'.root","cutsReco",'${markers[${comp2[count]}]}','${colors[${comp2[count]}]}','${markers[${comp1[count]}]}','${colors[${comp1[count]}]}',1.,1.,1.)';
	count=$count+1;	
    done
done
cp -r plots_PU* ~/www/dummyplots/production-timing/mtv_new/
