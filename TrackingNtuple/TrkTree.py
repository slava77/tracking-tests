from ROOT import TFile
from ROOT import gDirectory

# open the file
inputFile = TFile( 'ntuple.root' )

# retrieve the ntuple of interest
trkTree = gDirectory.Get( 'trkTree/tree' )
entries = trkTree.GetEntriesFast()

for jentry in xrange( entries ):
    
    # get the next tree in the chain and verify
    ientry = trkTree.LoadTree( jentry )
    if ientry < 0: break
    # copy next entry into memory and verify
    nb = trkTree.GetEntry( jentry )
    if nb <= 0: continue

    print "\nNEW EVENT"

    # use the values directly from the tree
    itk = 0
    for trkpt in trkTree.trk_pt:
        isd = trkTree.trk_seedIdx[itk]
        print ( "track %i algo=%i with pT=%f from seed idx=%i with pT=%f" % (itk,trkTree.trk_algo[itk],trkpt,isd,trkTree.see_pt[isd]) )
        itk = itk+1
