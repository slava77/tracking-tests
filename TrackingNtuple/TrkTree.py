from ROOT import TFile
from ROOT import gDirectory
import math

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

    # loop over reconstructed tracks
    itk = 0
    for trkpt in trkTree.trk_pt:
        algo = trkTree.trk_algo[itk]
        trketa = trkTree.trk_eta[itk]
        trkphi = trkTree.trk_phi[itk]
        itp = trkTree.trk_simIdx[itk]
        simpt = -1
        simeta = -1
        simphi = -1
        if itp >= 0: 
            simpt = trkTree.sim_pt[itp]
            simeta = trkTree.sim_eta[itp]
            simphi = trkTree.sim_phi[itp]
        print ( "track %i algo=%i with pT=%f eta=%f phi=%f matching simTrack idx=%i with pT=%f eta=%f phi=%f" % (itk,algo,trkpt,trketa,trkphi,itp,simpt,simeta,simphi) )
        #track hits
        for ipx in trkTree.trk_pixelIdx[itk]:
            x = trkTree.pix_x[ipx]
            y = trkTree.pix_y[ipx]
            z = trkTree.pix_z[ipx]
            r = math.sqrt(x*x + y*y)
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            xs = trkTree.pix_xsim[ipx]
            ys = trkTree.pix_ysim[ipx]
            zs = trkTree.pix_zsim[ipx]
            print ( "\tpixel pos=(%f,%f,%f) r=%f eta=%f phi=%f, sim pos=(%f,%f,%f)" % (x,y,z,r,eta,phi,xs,ys,zs) )
            ipx = ipx+1
        for ist in trkTree.trk_stripIdx[itk]:
            x = trkTree.str_x[ist]
            y = trkTree.str_y[ist]
            z = trkTree.str_z[ist]
            r = math.sqrt(x*x + y*y)
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            xs = trkTree.str_xsim[ist]
            ys = trkTree.str_ysim[ist]
            zs = trkTree.str_zsim[ist]
            print ( "\tstrip pos=(%f,%f,%f) r=%f eta=%f phi=%f, sim pos=(%f,%f,%f)" % (x,y,z,r,eta,phi,xs,ys,zs) )
            ist = ist+1
        #track seed
        isd = trkTree.trk_seedIdx[itk]
        seept = trkTree.see_pt[isd]
        seeeta = trkTree.see_eta[isd]
        seephi = trkTree.see_phi[isd]
        print ( "\tfrom seed idx=%i with pT=%f eta=%f phi=%f" % (isd,seept,seeeta,seephi) )
        #seed hits
        for ipx in trkTree.see_pixelIdx[isd]:
            x = trkTree.pix_x[ipx]
            y = trkTree.pix_y[ipx]
            z = trkTree.pix_z[ipx]
            r = math.sqrt(x*x + y*y)
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            print ( "\t\tpixel pos=(%f,%f,%f) r=%f eta=%f phi=%f" % (x,y,z,r,eta,phi) )
            ipx = ipx+1
        for igl in trkTree.see_gluedIdx[isd]:
            x = trkTree.glu_x[igl]
            y = trkTree.glu_y[igl]
            z = trkTree.glu_z[igl]
            r = math.sqrt(x*x + y*y)
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            print ( "\t\tglued pos=(%f,%f,%f) r=%f eta=%f phi=%f" % (x,y,z,r,eta,phi) )
            igl = igl+1
        for ist in trkTree.see_stripIdx[isd]:
            x = trkTree.str_x[ist]
            y = trkTree.str_y[ist]
            z = trkTree.str_z[ist]
            r = math.sqrt(x*x + y*y)
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            print ( "\t\tstrip pos=(%f,%f,%f) r=%f eta=%f phi=%f" % (x,y,z,r,eta,phi) )
            ist = ist+1
        itk = itk+1

