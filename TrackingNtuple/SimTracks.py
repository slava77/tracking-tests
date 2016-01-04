from ROOT import TFile, gDirectory, TH1F, TH2F
import math

# open the file
inputFile = TFile( 'ntuple_1GeV_1k.root' )

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

    # loop over simulated tracks
    for itp in range(0,len(trkTree.sim_pt)):
        q =  trkTree.sim_q[itp]
        px =  trkTree.sim_px[itp]
        py =  trkTree.sim_py[itp]
        pz =  trkTree.sim_pz[itp]
        pt = trkTree.sim_pt[itp]
        eta = trkTree.sim_eta[itp]
        phi = trkTree.sim_phi[itp]
        prodx =  trkTree.sim_prodx[itp]
        prody =  trkTree.sim_prody[itp]
        prodz =  trkTree.sim_prodz[itp]
        if math.fabs(eta)>0.6: continue
        if math.fabs(prodz)>5: continue        
        trkIdx = trkTree.sim_trkIdx[itp]
        trkAlgo = -1
        if trkIdx >= 0: trkAlgo = trkTree.trk_algo[trkIdx]
        hits = []
        #sim hits
        for ipix in trkTree.sim_pixelIdx[itp]:
            x = trkTree.pix_xsim[ipix]
            y = trkTree.pix_ysim[ipix]
            z = trkTree.pix_zsim[ipix]
            r = math.sqrt(x*x + y*y)
            pxsh = 0#trkTree.pix_pxsim[ipix]
            pysh = 0#trkTree.pix_pysim[ipix]
            pzsh = 0#trkTree.pix_pzsim[ipix]
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            det = 2-trkTree.pix_isBarrel[ipix]
            lay = trkTree.pix_lay[ipix]
            radL = trkTree.pix_radL[ipix]
            bbxi = trkTree.pix_bbxi[ipix]
            #
            nSimTrk   = trkTree.pix_nSimTrk  [ipix]
            simTrkIdx = trkTree.pix_simTrkIdx[ipix]
            particle  = trkTree.pix_particle [ipix]
            process   = trkTree.pix_process  [ipix]
            #print (" nSimTrk=%f simTrkIdx=%f particle=%f process=%f" % (nSimTrk,simTrkIdx,particle,process))
            if process!=0: continue
            #
            hits.append( (det,lay,x,y,z,r,eta,phi,radL,bbxi,pxsh,pysh,pzsh) )
            #print ( "\tpixel det=%i lay=%i sim pos=%f,%f,%f r=%f eta=%f phi=%f" % (det,lay,x,y,z,r,eta,phi) )
        for istr in trkTree.sim_stripIdx[itp]:
            #if trkTree.str_isStereo[istr]==1: continue
            x = trkTree.str_xsim[istr]
            y = trkTree.str_ysim[istr]
            z = trkTree.str_zsim[istr]
            r = math.sqrt(x*x + y*y)
            pxsh = 0#trkTree.str_pxsim[istr]
            pysh = 0#trkTree.str_pysim[istr]
            pzsh = 0#trkTree.str_pzsim[istr]
            eta = -math.log( math.tan( math.atan2(r,z)/2) )
            phi = math.atan2(y,x)
            det = trkTree.str_det[istr]
            lay = trkTree.str_lay[istr]
            radL = trkTree.str_radL[istr]
            bbxi = trkTree.str_bbxi[istr]
            #
            nSimTrk   = trkTree.str_nSimTrk  [istr]
            simTrkIdx = trkTree.str_simTrkIdx[istr]
            particle  = trkTree.str_particle [istr]
            process   = trkTree.str_process  [istr]
            #print (" nSimTrk=%f simTrkIdx=%f particle=%f process=%f" % (nSimTrk,simTrkIdx,particle,process))
            if process!=0: continue
            #
            hits.append( (det,lay,x,y,z,r,eta,phi,radL,bbxi,pxsh,pysh,pzsh) )
            #print ( "\tstrip det=%i lay=%i sim pos=%f,%f,%f r=%f eta=%f phi=%f" % (det,lay,x,y,z,r,eta,phi) )
        prev_det = -1
        prev_lay = -1
        totLayers = 0
        for hit in sorted(hits, key=lambda hit: hit[5]):
            if totLayers==0 and (hit[0]!=1 or hit[1]!=1): continue
            if hit[0]==prev_det and hit[1]==prev_lay: continue
            prev_det = hit[0]
            prev_lay = hit[1]
            totLayers = totLayers+1
        if totLayers < 13: continue
        #now we can print the info
        print ( "simTrack %f %f %f %f %f %f %i pt=%f trkIdx=%i algo=%i" % (prodx,prody,prodz,px,py,pz,q,pt,trkIdx,trkAlgo) )
        prev_det = -1
        prev_lay = -1
        for hit in sorted(hits, key=lambda hit: hit[5]):
            if hit[0]==prev_det and hit[1]==prev_lay: continue
            print  ( "simHit %f %f %f %f %f %f %f %f %f %f %f" % (hit[2],hit[3],hit[4],hit[5],hit[6],hit[8],hit[9],hit[10],hit[11],hit[12],math.sqrt(hit[10]*hit[10]+hit[11]*hit[11]+hit[12]*hit[12])) )
            prev_det = hit[0]
            prev_lay = hit[1]
