// -*- C++ -*-
//
// Package:    SeedAndTrackAnalyzer
// Class:      SeedAndTrackAnalyzer
// 
/**\class SeedAndTrackAnalyzer SeedAndTrackAnalyzer.cc Demo/SeedAndTrackAnalyzer/src/SeedAndTrackAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Giuseppe Cerati,28 S-012,+41227678302,
//         Created:  Tue Apr 16 13:22:56 CEST 2013
// $Id: SeedAndTrackAnalyzer.cc,v 1.1 2013/04/24 08:23:08 cerati Exp $
//
//

// skelethon generated with command: mkedanlzr -track SeedAndTrackAnalyzer

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "RecoPixelVertexing/PixelTrackFitting/src/RZLine.h"
#include <TH2F.h>

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

//
// class declaration
//

class SeedAndTrackAnalyzer : public edm::EDAnalyzer {
   public:
      explicit SeedAndTrackAnalyzer(const edm::ParameterSet&);
      ~SeedAndTrackAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      template<class T, class U> void makeFillHisto1D(const char* name,const char* title,int nbins,U minx,U maxx,U value);
      template<class T, class U> void makeFillHisto2D(const char* name,const char* title,int nbinsx,U minx,U maxx,U valuex,int nbinsy,U miny,U maxy,U valuey);

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      //virtual void endRun(edm::Run const&, edm::EventSetup const&);
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      std::vector<edm::InputTag> seedTags_; //used to select what seeds to read from configuration file
      std::vector<edm::InputTag> trackTags_; //used to select what tracks to read from configuration file
      std::string builderName_;
      edm::ESHandle<MagneticField> theMF;
      edm::ESHandle<TransientTrackingRecHitBuilder> theTTRHBuilder;
      bool debug;
      double minEta, maxEta;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SeedAndTrackAnalyzer::SeedAndTrackAnalyzer(const edm::ParameterSet& iConfig):
  seedTags_(iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("seeds")),
  trackTags_(iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("tracks")),
  builderName_(iConfig.getParameter<std::string>("TTRHBuilder")),
  debug(iConfig.getParameter<bool>("debug")),
  minEta(iConfig.getParameter<double>("minEta")),
  maxEta(iConfig.getParameter<double>("maxEta"))
{

  edm::Service<TFileService> fs;
  fs->make<TDirectory>("dummy","dummy");//do this so that the file is properly created

}


SeedAndTrackAnalyzer::~SeedAndTrackAnalyzer() {}

template<class T, class U> void SeedAndTrackAnalyzer::makeFillHisto1D(const char* name,const char* title,int nbins,U minx,U maxx,U value) {
  edm::Service<TFileService> fs;
  T* h = 0;
  try {
    h = fs->getObject<T>(name);
  } catch (cms::Exception e) {
    if (e.category()=="ObjectNotFound") {
      //std::cout << name <<" "<< title <<" "<< nbins <<" "<< minx <<" "<< maxx << std::endl;
      h = fs->make<T>(name, title, nbins, minx, maxx);
    } else throw e;
  }
  h->Fill(value);
}

template<class T, class U> void SeedAndTrackAnalyzer::makeFillHisto2D(const char* name,const char* title,int nbinsx,U minx,U maxx,U valuex,int nbinsy,U miny,U maxy,U valuey) {
  edm::Service<TFileService> fs;
  T* h = 0;
  try {
    h = fs->getObject<T>(name);
  } catch (cms::Exception e) {
    if (e.category()=="ObjectNotFound") {
      //std::cout << name <<" "<< title <<" "<< nbins <<" "<< minx <<" "<< maxx << std::endl;
      h = fs->make<T>(name, title, nbinsx, minx, maxx, nbinsy, miny, maxy);
    } else throw e;
  }
  h->Fill(valuex,valuey);
}

//
// member functions
//

// ------------ method called for each event  ------------
void SeedAndTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace std;
  using namespace edm;
  using namespace reco;

  //seeds
  for (unsigned int itsd=0;itsd<seedTags_.size();++itsd) {

    InputTag seedTag_ = seedTags_[itsd];
    Handle<TrajectorySeedCollection> seeds;
    iEvent.getByLabel(seedTag_,seeds);
    TString label = seedTag_.label();

    if (debug) cout << "NEW SEED LABEL: " << label << " size: " << seeds->size() << endl;

    int totSd = 0;
    for(TrajectorySeedCollection::const_iterator itSeed = seeds->begin(); itSeed != seeds->end(); ++itSeed,++totSd) {
      //find seed class at: http://cmssdt.cern.ch/SDT/lxr/source/DataFormats/TrajectorySeed/interface/TrajectorySeed.h?v=CMSSW_6_2_0_pre2

      TransientTrackingRecHit::RecHitPointer lastRecHit = theTTRHBuilder->build(&*(itSeed->recHits().second-1));
      TrajectoryStateOnSurface state = trajectoryStateTransform::transientState( itSeed->startingState(), lastRecHit->surface(), theMF.product());

      int charge = state.charge();  
      float pt  = state.globalParameters().momentum().perp();
      float eta = state.globalParameters().momentum().eta();  
      float phi = state.globalParameters().momentum().phi();  
      int nHits = itSeed->nHits();  

      makeFillHisto1D<TH1F,float>(TString(label+"_pt_sd").Data(),"pt_sd",1000,0,100,pt);
      makeFillHisto1D<TH1F,float>(TString(label+"_eta_sd").Data(),"eta_sd",100,-5,5,eta);
      makeFillHisto1D<TH1F,float>(TString(label+"_phi_sd").Data(),"phi_sd",64,-3.2,3.2,phi);
      makeFillHisto1D<TH1F,float>(TString(label+"_nHits_sd").Data(),"nHits_sd",51,-0.5,50.5,nHits);
      makeFillHisto1D<TH1F,float>(TString(label+"_charge_sd").Data(),"charge_sd",3,-1.5,1.5,charge);

      if (nHits==2) {
	TransientTrackingRecHit::RecHitPointer recHit0 = theTTRHBuilder->build(&*(itSeed->recHits().first));
	TransientTrackingRecHit::RecHitPointer recHit1 = theTTRHBuilder->build(&*(itSeed->recHits().first+1));
	vector<GlobalPoint> gp(2);
	vector<GlobalError> ge(2);
	gp[0] = recHit0->globalPosition();
	ge[0] = recHit0->globalPositionError();
	gp[1] = recHit1->globalPosition();
	ge[1] = recHit1->globalPositionError();
	if (debug) {
	  cout << "seed " << totSd << " - PAIR - ids: " << recHit0->geographicalId().rawId() << " " << recHit1->geographicalId().rawId()
	       << " hitpos: " << gp[0] << " " << gp[1]
	       << " trans0: " << (recHit0->transientHits().size()>1 ? recHit0->transientHits()[0]->globalPosition() : GlobalPoint(0,0,0)) 
	       << " " << (recHit0->transientHits().size()>1 ? recHit0->transientHits()[1]->globalPosition() : GlobalPoint(0,0,0))
	       << " trans1: " << (recHit1->transientHits().size()>1 ? recHit1->transientHits()[0]->globalPosition() : GlobalPoint(0,0,0)) 
	       << " " << (recHit1->transientHits().size()>1 ? recHit1->transientHits()[1]->globalPosition() : GlobalPoint(0,0,0))
	       << " eta,phi: " << gp[0].eta() << "," << gp[0].phi()  
	       << endl;
	}
      } else if (nHits==3) {
	TransientTrackingRecHit::RecHitPointer recHit0 = theTTRHBuilder->build(&*(itSeed->recHits().first));
	TransientTrackingRecHit::RecHitPointer recHit1 = theTTRHBuilder->build(&*(itSeed->recHits().first+1));
	TransientTrackingRecHit::RecHitPointer recHit2 = theTTRHBuilder->build(&*(itSeed->recHits().first+2));

	//compute chi2 from RZLine
	/*
	  vector<float> r(3),z(3),errZ(3);
	  r[0] = recHit0->globalPosition().perp();
	  z[0] = recHit0->globalPosition().z();
	  errZ[0] = sqrt(recHit0->globalPositionError().czz());
	  r[1] = recHit1->globalPosition().perp();
	  z[1] = recHit1->globalPosition().z();
	  errZ[1] = sqrt(recHit1->globalPositionError().czz());
	  r[2] = recHit2->globalPosition().perp();
	  z[2] = recHit2->globalPosition().z();
	  errZ[2] = sqrt(recHit2->globalPositionError().czz());
	  RZLine rzLine(r,z,errZ);
	*/
	vector<GlobalPoint> gp(3);
	vector<GlobalError> ge(3);
	vector<bool> bl(3);
	gp[0] = recHit0->globalPosition();
	ge[0] = recHit0->globalPositionError();
	int subid0 = recHit0->geographicalId().subdetId();
	bl[0] = (subid0 == StripSubdetector::TIB || subid0 == StripSubdetector::TOB || subid0 == (int) PixelSubdetector::PixelBarrel);
	gp[1] = recHit1->globalPosition();
	ge[1] = recHit1->globalPositionError();
	int subid1 = recHit1->geographicalId().subdetId();
	bl[1] = (subid1 == StripSubdetector::TIB || subid1 == StripSubdetector::TOB || subid1 == (int) PixelSubdetector::PixelBarrel);
	gp[2] = recHit2->globalPosition();
	ge[2] = recHit2->globalPositionError();
	int subid2 = recHit2->geographicalId().subdetId();
	bl[2] = (subid2 == StripSubdetector::TIB || subid2 == StripSubdetector::TOB || subid2 == (int) PixelSubdetector::PixelBarrel);
	RZLine rzLine(gp,ge,bl);
	float  cottheta, intercept, covss, covii, covsi;
	rzLine.fit(cottheta, intercept, covss, covii, covsi);
	float seed_chi2 = rzLine.chi2(cottheta, intercept);	    
	float seed_pt = state.globalParameters().momentum().perp();
	makeFillHisto1D<TH1F,float>(TString(label+"_chi2_seed").Data(),"chi2_seed",1000,0,100,seed_chi2);
	makeFillHisto1D<TH1F,float>(TString(label+"_pt_seed").Data(),"pt_seed",1000,0,100,seed_pt);
	makeFillHisto2D<TH2F,float>(TString(label+"_chi2_vs_pt_seed").Data(),"chi2_vs_pt_seed",1000,0,100,seed_pt,1000,0,100,seed_chi2);
	
	if (debug) {
	  cout << "seed " << totSd << " - TRIPLET - ids: " << recHit0->geographicalId().rawId() << " " << recHit1->geographicalId().rawId() << " " << recHit2->geographicalId().rawId()
	       << " hitpos: " << gp[0] << " " << gp[1] << " " << gp[2] 
	       << " trans0: " << (recHit0->transientHits().size()>1 ? recHit0->transientHits()[0]->globalPosition() : GlobalPoint(0,0,0)) 
	       << " " << (recHit0->transientHits().size()>1 ? recHit0->transientHits()[1]->globalPosition() : GlobalPoint(0,0,0))
	       << " trans1: " << (recHit1->transientHits().size()>1 ? recHit1->transientHits()[0]->globalPosition() : GlobalPoint(0,0,0)) 
	       << " " << (recHit1->transientHits().size()>1 ? recHit1->transientHits()[1]->globalPosition() : GlobalPoint(0,0,0))
	       << " trans2: " << (recHit2->transientHits().size()>1 ? recHit2->transientHits()[0]->globalPosition() : GlobalPoint(0,0,0)) 
	       << " " << (recHit2->transientHits().size()>1 ? recHit2->transientHits()[1]->globalPosition() : GlobalPoint(0,0,0))
	       << " local: " << recHit2->localPosition()
	       << " tsos pos, mom: " << state.globalPosition()<<" "<<state.globalMomentum()
	       << " eta,phi: " << gp[0].eta() << "," << gp[0].phi()  
	       << " pt,chi2: " << seed_pt << "," << seed_chi2 << endl;
	}
      }
    }
    makeFillHisto1D<TH1F,int>(TString(label+"_nsd").Data(),"nsd",100,0,5000,totSd);
  }


  //tracks
  for (unsigned int ittk=0;ittk<trackTags_.size();++ittk) {

    InputTag trackTag_ = trackTags_[ittk];
    Handle<TrackCollection> tracks;
    iEvent.getByLabel(trackTag_,tracks);
    TString label = trackTag_.label();

    if (debug) cout << "NEW TRACK LABEL: " << label << endl;

    int totTk = 0, totHp = 0;
    for(TrackCollection::const_iterator itTrack = tracks->begin(); itTrack != tracks->end(); ++itTrack, ++totTk) {
      //find track class at: http://cmssdt.cern.ch/SDT/lxr/source/DataFormats/TrackReco/interface/Track.h?v=CMSSW_6_2_0_pre2
      
      int charge = itTrack->charge();  
      float pt = itTrack->pt();  
      float eta = itTrack->eta();  
      float chi2 = itTrack->normalizedChi2();  
      float phi = itTrack->phi();  
      int nHits = itTrack->numberOfValidHits();  
      float rho = itTrack->innerPosition().rho();
      HitPattern hp = itTrack->hitPattern();

      if (pt<0.4 || fabs(eta)<minEta || fabs(eta)>maxEta) continue;
      //focus on the tracks that we can try to reconstruct with triplets
      //if (hp.numberOfValidStripLayersWithMonoAndStereo()+hp.pixelLayersWithMeasurement()<3) continue;
      //if (hp.numberOfValidStripLayersWithMonoAndStereo()<3) continue;
      //if (hp.pixelLayersWithMeasurement()<1) continue;
      //if (itTrack->quality(TrackBase::highPurity)==0) continue;

      //if (debug) {
      if (debug && itTrack->quality(TrackBase::highPurity)!=0) {
	cout << "Track #" << totTk << " with q=" << itTrack->charge() 
	     << ", pT=" << itTrack->pt() << " GeV, eta: " << itTrack->eta() << ", phi: " << itTrack->phi() 
	     << ", Nhits=" << itTrack->recHitsSize() 
	     << ", algo=" << itTrack->algoName(itTrack->algo()).c_str() 
	     << " hp=" << itTrack->quality(TrackBase::highPurity) 
	     << " seed#=" << itTrack->seedRef().key()
	     << endl;
	int nhit = 0;
	for (trackingRecHit_iterator i=itTrack->recHitsBegin(); i!=itTrack->recHitsEnd(); i++){
	  cout << "hit #" << nhit;
	  TransientTrackingRecHit::RecHitPointer hit=theTTRHBuilder->build(&**i );
	  DetId hitId = hit->geographicalId();
	  cout << " subdet=" << hitId.subdetId();
	  if(hitId.det() == DetId::Tracker) {
	    if (hitId.subdetId() == StripSubdetector::TIB )  
	      cout << " - TIB " << TIBDetId(hitId).layer();
	    else if (hitId.subdetId() == StripSubdetector::TOB ) 
	      cout << " - TOB " << TOBDetId(hitId).layer();
	    else if (hitId.subdetId() == StripSubdetector::TEC ) 
	      cout << " - TEC " << TECDetId(hitId).wheel();
	    else if (hitId.subdetId() == StripSubdetector::TID ) 
	      cout << " - TID " << TIDDetId(hitId).wheel();
	    else if (hitId.subdetId() == StripSubdetector::TID ) 
	      cout << " - TID " << TIDDetId(hitId).wheel();
	    else if (hitId.subdetId() == (int) PixelSubdetector::PixelBarrel ) 
	      cout << " - PixBar " << PXBDetId(hitId).layer();
	    else if (hitId.subdetId() == (int) PixelSubdetector::PixelEndcap )
	      cout << " - PixFwd " << PXFDetId(hitId).disk();
	    else 
	      cout << " UNKNOWN TRACKER HIT TYPE ";
	  }
	  if (hit->isValid()) cout << " id: " << hitId.rawId() << " - globalPos =" << hit->globalPosition() << " eta,phi: " << hit->globalPosition().eta() << "," << hit->globalPosition().phi()  << endl;
	  else cout << " - invalid hit" << endl;
	  nhit++;
	}    
	cout << endl;
      }
      
      makeFillHisto1D<TH1F,float>(TString(label+"_pt_tk").Data(),"pt_tk",1000,0,100,pt);
      makeFillHisto1D<TH1F,float>(TString(label+"_eta_tk").Data(),"eta_tk",100,-5,5,eta);
      makeFillHisto1D<TH1F,float>(TString(label+"_phi_tk").Data(),"phi_tk",64,-3.2,3.2,phi);
      makeFillHisto1D<TH1F,float>(TString(label+"_chi2_tk").Data(),"chi2_tk",1000,0,100,chi2);
      makeFillHisto1D<TH1F,float>(TString(label+"_nHits_tk").Data(),"nHits_tk",51,-0.5,50.5,nHits);
      makeFillHisto1D<TH1F,float>(TString(label+"_charge_tk").Data(),"charge_tk",3,-1.5,1.5,charge);
      makeFillHisto1D<TH1F,float>(TString(label+"_rho_tk").Data(),"rho_tk",100,0,100,rho);
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_pixel_tk").Data(),"nlayer_pixel",11,-0.5,10.5,hp.pixelLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_strip_tk").Data(),"nlayer_strip",31,-0.5,30.5,hp.stripLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_strip_3d_tk").Data(),"nlayer_strip_3d",11,-0.5,10.5,hp.numberOfValidStripLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tot_3d_tk").Data(),"nlayer_tot_3d",16,-0.5,15.5,hp.pixelLayersWithMeasurement()+hp.numberOfValidStripLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_pxb_tk").Data(),"nlayer_pxb",11,-0.5,10.5,hp.pixelBarrelLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_pxf_tk").Data(),"nlayer_pxf",11,-0.5,10.5,hp.pixelEndcapLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tib_tk").Data(),"nlayer_tib",11,-0.5,10.5,hp.stripTIBLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tob_tk").Data(),"nlayer_tob",11,-0.5,10.5,hp.stripTOBLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid_tk").Data(),"nlayer_tid",11,-0.5,10.5,hp.stripTIDLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec_tk").Data(),"nlayer_tec",11,-0.5,10.5,hp.stripTECLayersWithMeasurement());
      /*
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tib_3d_tk").Data(),"nlayer_tib_3d_tk",11,-0.5,10.5,hp.numberOfValidTIBLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tob_3d_tk").Data(),"nlayer_tob_3d_tk",11,-0.5,10.5,hp.numberOfValidTOBLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid_3d_tk").Data(),"nlayer_tid_3d_tk",11,-0.5,10.5,hp.numberOfValidTIDLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec_3d_tk").Data(),"nlayer_tec_3d_tk",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo());
      */

      //use only highPurity tracks
      if (itTrack->quality(TrackBase::highPurity)==0) continue;
      totHp++;
      makeFillHisto1D<TH1F,float>(TString(label+"_pt_hp").Data(),"pt_hp",1000,0,100,pt);
      makeFillHisto1D<TH1F,float>(TString(label+"_eta_hp").Data(),"eta_hp",100,-5,5,eta);
      makeFillHisto1D<TH1F,float>(TString(label+"_phi_hp").Data(),"phi_hp",64,-3.2,3.2,phi);
      makeFillHisto1D<TH1F,float>(TString(label+"_chi2_hp").Data(),"chi2_hp",1000,0,100,chi2);
      makeFillHisto1D<TH1F,float>(TString(label+"_nHits_hp").Data(),"nHits_hp",51,-0.5,50.5,nHits);
      makeFillHisto1D<TH1F,float>(TString(label+"_charge_hp").Data(),"charge_hp",3,-1.5,1.5,charge);
      makeFillHisto1D<TH1F,float>(TString(label+"_rho_hp").Data(),"rho_hp",100,0,100,rho);
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_pixel_hp").Data(),"nlayer_pixel",11,-0.5,10.5,hp.pixelLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_strip_hp").Data(),"nlayer_strip",31,-0.5,30.5,hp.stripLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_strip_3d_hp").Data(),"nlayer_strip_3d",11,-0.5,10.5,hp.numberOfValidStripLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tot_3d_hp").Data(),"nlayer_tot_3d",16,-0.5,15.5,hp.pixelLayersWithMeasurement()+hp.numberOfValidStripLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_pxb_hp").Data(),"nlayer_pxb",11,-0.5,10.5,hp.pixelBarrelLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_pxf_hp").Data(),"nlayer_pxf",11,-0.5,10.5,hp.pixelEndcapLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tib_hp").Data(),"nlayer_tib",11,-0.5,10.5,hp.stripTIBLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tob_hp").Data(),"nlayer_tob",11,-0.5,10.5,hp.stripTOBLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid_hp").Data(),"nlayer_tid",11,-0.5,10.5,hp.stripTIDLayersWithMeasurement());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec_hp").Data(),"nlayer_tec",11,-0.5,10.5,hp.stripTECLayersWithMeasurement());
      /*
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tib_3d_hp").Data(),"nlayer_tib_3d_hp",11,-0.5,10.5,hp.numberOfValidTIBLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tob_3d_hp").Data(),"nlayer_tob_3d_hp",11,-0.5,10.5,hp.numberOfValidTOBLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid_3d_hp").Data(),"nlayer_tid_3d_hp",11,-0.5,10.5,hp.numberOfValidTIDLayersWithMonoAndStereo());
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec_3d_hp").Data(),"nlayer_tec_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo());

      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid1_3d_hp").Data(),"nlayer_tid1_3d_hp",11,-0.5,10.5,hp.numberOfValidTIDLayersWithMonoAndStereo(1));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid2_3d_hp").Data(),"nlayer_tid2_3d_hp",11,-0.5,10.5,hp.numberOfValidTIDLayersWithMonoAndStereo(2));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid3_3d_hp").Data(),"nlayer_tid3_3d_hp",11,-0.5,10.5,hp.numberOfValidTIDLayersWithMonoAndStereo(3));

      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec1_3d_hp").Data(),"nlayer_tec1_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(1));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec2_3d_hp").Data(),"nlayer_tec2_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(2));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec3_3d_hp").Data(),"nlayer_tec3_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(3));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec4_3d_hp").Data(),"nlayer_tec4_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(4));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec5_3d_hp").Data(),"nlayer_tec5_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(5));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec6_3d_hp").Data(),"nlayer_tec6_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(6));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec7_3d_hp").Data(),"nlayer_tec7_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(7));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec8_3d_hp").Data(),"nlayer_tec8_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(8));
      makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tec9_3d_hp").Data(),"nlayer_tec9_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(9));

      if (hp.numberOfValidTIDLayersWithMonoAndStereo()==2 && hp.numberOfValidTIDLayersWithMonoAndStereo(1)==1  && hp.numberOfValidTIDLayersWithMonoAndStereo(2)==1 ) {
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec1_3d_hp").Data(),"nlayer_tid12tec1_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(1));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec2_3d_hp").Data(),"nlayer_tid12tec2_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(2));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec3_3d_hp").Data(),"nlayer_tid12tec3_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(3));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec4_3d_hp").Data(),"nlayer_tid12tec4_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(4));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec5_3d_hp").Data(),"nlayer_tid12tec5_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(5));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec6_3d_hp").Data(),"nlayer_tid12tec6_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(6));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec7_3d_hp").Data(),"nlayer_tid12tec7_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(7));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec8_3d_hp").Data(),"nlayer_tid12tec8_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(8));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid12tec9_3d_hp").Data(),"nlayer_tid12tec9_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(9));
      }

      if (hp.numberOfValidTIDLayersWithMonoAndStereo()==2 && hp.numberOfValidTIDLayersWithMonoAndStereo(2)==1  && hp.numberOfValidTIDLayersWithMonoAndStereo(3)==1 ) {
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec1_3d_hp").Data(),"nlayer_tid23tec1_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(1));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec2_3d_hp").Data(),"nlayer_tid23tec2_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(2));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec3_3d_hp").Data(),"nlayer_tid23tec3_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(3));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec4_3d_hp").Data(),"nlayer_tid23tec4_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(4));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec5_3d_hp").Data(),"nlayer_tid23tec5_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(5));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec6_3d_hp").Data(),"nlayer_tid23tec6_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(6));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec7_3d_hp").Data(),"nlayer_tid23tec7_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(7));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec8_3d_hp").Data(),"nlayer_tid23tec8_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(8));
	makeFillHisto1D<TH1F,float>(TString(label+"_nlayer_tid23tec9_3d_hp").Data(),"nlayer_tid23tec9_3d_hp",11,-0.5,10.5,hp.numberOfValidTECLayersWithMonoAndStereo(9));
      }
      */

      edm::RefToBase<TrajectorySeed> trackSeed = itTrack->seedRef();
      if (trackSeed.isNull()==0 && trackSeed->nHits()==3) {
	TransientTrackingRecHit::RecHitPointer recHit0 = theTTRHBuilder->build(&*(trackSeed->recHits().first));
	TransientTrackingRecHit::RecHitPointer recHit1 = theTTRHBuilder->build(&*(trackSeed->recHits().first+1));
	TransientTrackingRecHit::RecHitPointer recHit2 = theTTRHBuilder->build(&*(trackSeed->recHits().first+2));
	TrajectoryStateOnSurface state = trajectoryStateTransform::transientState( trackSeed->startingState(), recHit2->surface(), theMF.product());
	//compute chi2 from RZLine
	/*
	  vector<float> r(3),z(3),errZ(3);
	  r[0] = recHit0->globalPosition().perp();
	  z[0] = recHit0->globalPosition().z();
	  errZ[0] = sqrt(recHit0->globalPositionError().czz());
	  r[1] = recHit1->globalPosition().perp();
	  z[1] = recHit1->globalPosition().z();
	  errZ[1] = sqrt(recHit1->globalPositionError().czz());
	  r[2] = recHit2->globalPosition().perp();
	  z[2] = recHit2->globalPosition().z();
	  errZ[2] = sqrt(recHit2->globalPositionError().czz());
	  RZLine rzLine(r,z,errZ);
	*/
	vector<GlobalPoint> gp(3);
	vector<GlobalError> ge(3);
	vector<bool> bl(3);
	gp[0] = recHit0->globalPosition();
	ge[0] = recHit0->globalPositionError();
	int subid0 = recHit0->geographicalId().subdetId();
	bl[0] = (subid0 == StripSubdetector::TIB || subid0 == StripSubdetector::TOB || subid0 == (int) PixelSubdetector::PixelBarrel);
	gp[1] = recHit1->globalPosition();
	ge[1] = recHit1->globalPositionError();
	int subid1 = recHit1->geographicalId().subdetId();
	bl[1] = (subid1 == StripSubdetector::TIB || subid1 == StripSubdetector::TOB || subid1 == (int) PixelSubdetector::PixelBarrel);
	gp[2] = recHit2->globalPosition();
	ge[2] = recHit2->globalPositionError();
	int subid2 = recHit2->geographicalId().subdetId();
	bl[2] = (subid2 == StripSubdetector::TIB || subid2 == StripSubdetector::TOB || subid2 == (int) PixelSubdetector::PixelBarrel);
	RZLine rzLine(gp,ge,bl);
	float  cottheta, intercept, covss, covii, covsi;
	rzLine.fit(cottheta, intercept, covss, covii, covsi);
	float seed_chi2 = rzLine.chi2(cottheta, intercept);	    
	float seed_pt = state.globalParameters().momentum().perp();
	makeFillHisto1D<TH1F,float>(TString(label+"_chi2_seed").Data(),"chi2_seed",1000,0,100,seed_chi2);
	makeFillHisto1D<TH1F,float>(TString(label+"_pt_seed").Data(),"pt_seed",1000,0,100,seed_pt);
	makeFillHisto2D<TH2F,float>(TString(label+"_chi2_vs_pt_seed").Data(),"chi2_vs_pt_seed",1000,0,100,seed_pt,1000,0,100,seed_chi2);
	
	//cout << "track q/pt/eta/phi/nhits: " << charge << " / " << pt << " / " << eta << " / " << phi << " / " << nHits;
	//cout << " --- seed number: " << trackSeed.key() << " pt=" << state.globalMomentum().perp() << endl;
      }
    }
    makeFillHisto1D<TH1F,int>(TString(label+"_ntk").Data(),"ntk",100,0,5000,totTk);
    makeFillHisto1D<TH1F,int>(TString(label+"_nhp").Data(),"nhp",100,0,5000,totHp);
  }

}


// ------------ method called once each job just before starting event loop  ------------
void SeedAndTrackAnalyzer::beginJob() {}

// ------------ method called once each job just after ending the event loop  ------------
void SeedAndTrackAnalyzer::endJob() {}

// ------------ method called when starting to processes a run  ------------
void SeedAndTrackAnalyzer::beginRun(edm::Run const&, edm::EventSetup const& iSetup) {
  iSetup.get<IdealMagneticFieldRecord>().get(theMF);  
  iSetup.get<TransientRecHitRecord>().get(builderName_,theTTRHBuilder);
}

// ------------ method called when ending the processing of a run  ------------
//void SeedAndTrackAnalyzer::endRun(edm::Run const&, edm::EventSetup const&) {}

// ------------ method called when starting to processes a luminosity block  ------------
//void SeedAndTrackAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){}

// ------------ method called when ending the processing of a luminosity block  ------------
//void SeedAndTrackAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void SeedAndTrackAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SeedAndTrackAnalyzer);
