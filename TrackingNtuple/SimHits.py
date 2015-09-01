from ROOT import TFile, gDirectory, TH1F, TH2F
import math

# open the file
inputFile = TFile( 'ntuple.root' )

# retrieve the ntuple of interest
trkTree = gDirectory.Get( 'trkTree/tree' )
entries = trkTree.GetEntriesFast()

h_r = TH1F("h_r","h_r",1200,0,120)
h_xy = TH2F("h_xy","h_xy",3000,-150,150,3000,-150,150)
h_axy = TH2F("h_axy","h_axy",1500,0,150,1500,0,150)

for jentry in xrange( entries ):
    
    # get the next tree in the chain and verify
    ientry = trkTree.LoadTree( jentry )
    if ientry < 0: break
    # copy next entry into memory and verify
    nb = trkTree.GetEntry( jentry )
    if nb <= 0: continue

    print "\nNEW EVENT"

    # loop over pixel hits
    ipix = -1
    for ipixx in trkTree.pix_x:
        ipix = ipix+1
        if trkTree.pix_isBarrel[ipix]==0: continue
        x = trkTree.pix_x[ipix]
        y = trkTree.pix_y[ipix]
        z = trkTree.pix_z[ipix]
        r = math.sqrt(x*x + y*y)
        eta = -math.log( math.tan( math.atan2(r,z)/2) )
        phi = math.atan2(y,x)
        xs = trkTree.pix_xsim[ipix]
        ys = trkTree.pix_ysim[ipix]
        zs = trkTree.pix_zsim[ipix]
        print ( "\tpixel pos=(%f,%f,%f) r=%f eta=%f phi=%f, sim pos=(%f,%f,%f)" % (x,y,z,r,eta,phi,xs,ys,zs) )
        h_r.Fill(r)
        h_xy.Fill(xs,ys)
        h_axy.Fill(math.fabs(xs),math.fabs(ys))

    # loop over strip hits
    istr = -1
    for istrx in trkTree.str_x:
        istr = istr+1
        if trkTree.str_isBarrel[istr]==0: continue
        #if trkTree.str_isStereo[istr]==0: continue
        x = trkTree.str_x[istr]
        y = trkTree.str_y[istr]
        z = trkTree.str_z[istr]
        r = math.sqrt(x*x + y*y)
        eta = -math.log( math.tan( math.atan2(r,z)/2) )
        phi = math.atan2(y,x)
        xs = trkTree.str_xsim[istr]
        ys = trkTree.str_ysim[istr]
        zs = trkTree.str_zsim[istr]
        print ( "\tstrip pos=(%f,%f,%f) r=%f eta=%f phi=%f, sim pos=(%f,%f,%f), isStereo=%i" % (x,y,z,r,eta,phi,xs,ys,zs,trkTree.str_isStereo[istr]) )
        h_r.Fill(r)
        h_xy.Fill(xs,ys)
        h_axy.Fill(math.fabs(xs),math.fabs(ys))

h_r.Draw()

