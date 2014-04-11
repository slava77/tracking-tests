// -*- C++ -*-
//
// Package:    TestHits
// Class:      TestHits
// 
/**\class TestHits TestHits.cc TrackingTests/TrackTest/src/TestHits.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Giuseppe Cerati,28 S-012,+41227678302,
//         Created:  Wed Jul 10 10:12:46 CEST 2013
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/ContainerMask.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" //
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
// class declaration
//

class TestHits : public edm::EDAnalyzer {
   public:
      explicit TestHits(const edm::ParameterSet&);
      ~TestHits();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      template<class T, class U> void makeFillHisto1D(const char* name,const char* title,int nbins,U minx,U maxx,U value);

      std::string builderName;
      const TransientTrackingRecHitBuilder* builder;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      //virtual void endRun(edm::Run const&, edm::EventSetup const&);
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
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
TestHits::TestHits(const edm::ParameterSet& iConfig):
  builderName(iConfig.getParameter<std::string>("TTRHBuilder")) {

   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  fs->make<TDirectory>("dummy","dummy");

}


TestHits::~TestHits()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

template<class T, class U> void TestHits::makeFillHisto1D(const char* name,const char* title,int nbins,U minx,U maxx,U value) {
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

//
// member functions
//

// ------------ method called for each event  ------------
void
TestHits::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

   ESHandle<TransientTrackingRecHitBuilder> theBuilder;
   iSetup.get<TransientRecHitRecord>().get(builderName,theBuilder);
   builder=theBuilder.product();

   ESHandle<TrackerGeometry> geom;
   iSetup.get<TrackerDigiGeometryRecord>().get( geom );
   const TrackerGeometry& theTracker(*geom);

   map<int,pair<LocalPoint,GlobalPoint> > detsAndSimHits;

   edm::Handle<std::vector<PSimHit> > simhits; 
   //InputTag tags("g4SimHits","TrackerHitsPixelBarrelLowTof");
   InputTag tags("g4SimHits","TrackerHitsPixelEndcapLowTof");
   iEvent.getByLabel(tags, simhits);
   double dp_sim = -1;
   for (auto isim=simhits->begin(); isim!=simhits->end();isim++) {
     //totalNumOfSimHits++;
     DetId detId=DetId((*isim).detUnitId());
     const PixelGeomDetUnit * theGeomDet =  dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) );
     float x = (*isim).entryPoint().x(); // width (row index, in col direction)
     float y = (*isim).entryPoint().y(); // length (col index, in row direction) 
     float z = (*isim).entryPoint().z(); // thick
     float x2 = (*isim).exitPoint().x();
     float y2 = (*isim).exitPoint().y();
     float z2 = (*isim).exitPoint().z();
     float xpos = (x+x2)/2.;
     float ypos = (y+y2)/2.;
     float zpos = (z+z2)/2.;
     LocalPoint loc(xpos,ypos,zpos);
     GlobalPoint glo = theGeomDet->surface().toGlobal(loc);
     if (isim->processType()==2) dp_sim=simhits->begin()->pabs()-isim->pabs();
     cout << "simHit id=" << detId.rawId() << " gp=" << glo << " r=" << glo.perp() << " proc:" << isim->processType() << " eloss=" << isim->energyLoss() << " p=" << isim->pabs() ;//<< endl;
     cout << " disk-blade-panel: " << PXFDetId(detId.rawId()).disk() << "-" << PXFDetId(detId.rawId()).blade() << "-" << PXFDetId(detId.rawId()).panel() << endl;
     detsAndSimHits.insert( make_pair<int,pair<LocalPoint,GlobalPoint> >(int(detId.rawId()),make_pair<LocalPoint,GlobalPoint>(LocalPoint(loc),GlobalPoint(glo))) );
   }
   cout << "dp_sim=" << dp_sim << endl;
   makeFillHisto1D<TH1F,double>("dp_sim","dp_sim",100,0.,0.2,dp_sim);

   edm::Handle<SiPixelRecHitCollection> pixelHits;
   iEvent.getByLabel("siPixelRecHits", pixelHits);
   for (auto it = pixelHits->begin(); it!=pixelHits->end(); it++ ) {
     DetId hitId = it->detId();
     LocalPoint simHitLP = detsAndSimHits[hitId].first;
     GlobalPoint simHitGP = detsAndSimHits[hitId].second;
     auto match = it->end(); double dist = 99999;
     for (auto hit = it->begin(); hit!=it->end(); hit++ ) {
       TransientTrackingRecHit::RecHitPointer ttrh = builder->build(&*hit);
       std::cout << "id=" << hitId.rawId() << " gp=" << ttrh->globalPosition() << " r=" << ttrh->globalPosition().perp() 
		 << " lp=" << ttrh->localPosition() << " lpe:" << ttrh->localPositionError()<< std::endl;
       LocalVector hitDelta = ttrh->localPosition()-simHitLP;
       if (hitDelta.mag()<dist) {
	 dist = hitDelta.mag();
	 match = hit;
       }
     }

     if (match!=it->end()) {
       TransientTrackingRecHit::RecHitPointer ttrh = builder->build(&*match);
       cout << PXBDetId(hitId).layer() << " " << ttrh->globalPosition() << " " << simHitGP << endl;
       if (PXBDetId(hitId).layer()==1) {
	 makeFillHisto1D<TH1F,double>("l01_res_loc_x","l01_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l01_pul_loc_x","l01_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l01_res_loc_y","l01_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l01_pul_loc_y","l01_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l01_res_glo_x","l01_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l01_pul_glo_x","l01_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l01_res_glo_y","l01_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l01_pul_glo_y","l01_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l01_res_glo_z","l01_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l01_pul_glo_z","l01_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
       } else if (PXBDetId(hitId).layer()==2) {
	 makeFillHisto1D<TH1F,double>("l02_res_loc_x","l02_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l02_pul_loc_x","l02_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l02_res_loc_y","l02_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l02_pul_loc_y","l02_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l02_res_glo_x","l02_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l02_pul_glo_x","l02_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l02_res_glo_y","l02_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l02_pul_glo_y","l02_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l02_res_glo_z","l02_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l02_pul_glo_z","l02_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
       } else if (PXBDetId(hitId).layer()==3) {
	 makeFillHisto1D<TH1F,double>("l03_res_loc_x","l03_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l03_pul_loc_x","l03_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l03_res_loc_y","l03_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l03_pul_loc_y","l03_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l03_res_glo_x","l03_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l03_pul_glo_x","l03_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l03_res_glo_y","l03_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l03_pul_glo_y","l03_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l03_res_glo_z","l03_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l03_pul_glo_z","l03_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
       } else if (PXBDetId(hitId).layer()==4) {
	 makeFillHisto1D<TH1F,double>("l04_res_loc_x","l04_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l04_pul_loc_x","l04_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l04_res_loc_y","l04_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l04_pul_loc_y","l04_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l04_res_glo_x","l04_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l04_pul_glo_x","l04_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l04_res_glo_y","l04_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l04_pul_glo_y","l04_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l04_res_glo_z","l04_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l04_pul_glo_z","l04_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
       } else if (PXBDetId(hitId).layer()==5) {
	 makeFillHisto1D<TH1F,double>("l05_res_loc_x","l05_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l05_pul_loc_x","l05_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l05_res_loc_y","l05_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l05_pul_loc_y","l05_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l05_res_glo_x","l05_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l05_pul_glo_x","l05_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l05_res_glo_y","l05_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l05_pul_glo_y","l05_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l05_res_glo_z","l05_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l05_pul_glo_z","l05_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 if (sqrt(match->localPositionError().yy())<0.1) {
	   makeFillHisto1D<TH1F,double>("l05p_res_loc_x","l05p_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l05p_pul_loc_x","l05p_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l05p_res_loc_y","l05p_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l05p_pul_loc_y","l05p_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l05p_res_glo_x","l05p_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l05p_pul_glo_x","l05p_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l05p_res_glo_y","l05p_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l05p_pul_glo_y","l05p_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l05p_res_glo_z","l05p_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l05p_pul_glo_z","l05p_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 } else {
	   makeFillHisto1D<TH1F,double>("l05s_res_loc_x","l05s_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l05s_pul_loc_x","l05s_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l05s_res_loc_y","l05s_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l05s_pul_loc_y","l05s_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l05s_res_glo_x","l05s_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l05s_pul_glo_x","l05s_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l05s_res_glo_y","l05s_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l05s_pul_glo_y","l05s_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l05s_res_glo_z","l05s_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l05s_pul_glo_z","l05s_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 }
       } else if (PXBDetId(hitId).layer()==6) {
	 makeFillHisto1D<TH1F,double>("l06_res_loc_x","l06_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l06_pul_loc_x","l06_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l06_res_loc_y","l06_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l06_pul_loc_y","l06_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l06_res_glo_x","l06_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l06_pul_glo_x","l06_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l06_res_glo_y","l06_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l06_pul_glo_y","l06_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l06_res_glo_z","l06_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l06_pul_glo_z","l06_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 if (sqrt(match->localPositionError().yy())<0.1) {
	   makeFillHisto1D<TH1F,double>("l06p_res_loc_x","l06p_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l06p_pul_loc_x","l06p_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l06p_res_loc_y","l06p_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l06p_pul_loc_y","l06p_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l06p_res_glo_x","l06p_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l06p_pul_glo_x","l06p_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l06p_res_glo_y","l06p_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l06p_pul_glo_y","l06p_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l06p_res_glo_z","l06p_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l06p_pul_glo_z","l06p_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 } else {
	   makeFillHisto1D<TH1F,double>("l06s_res_loc_x","l06s_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l06s_pul_loc_x","l06s_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l06s_res_loc_y","l06s_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l06s_pul_loc_y","l06s_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l06s_res_glo_x","l06s_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l06s_pul_glo_x","l06s_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l06s_res_glo_y","l06s_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l06s_pul_glo_y","l06s_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l06s_res_glo_z","l06s_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l06s_pul_glo_z","l06s_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 }
       } else if (PXBDetId(hitId).layer()==7) {
	 makeFillHisto1D<TH1F,double>("l07_res_loc_x","l07_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l07_pul_loc_x","l07_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l07_res_loc_y","l07_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l07_pul_loc_y","l07_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l07_res_glo_x","l07_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l07_pul_glo_x","l07_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l07_res_glo_y","l07_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l07_pul_glo_y","l07_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l07_res_glo_z","l07_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l07_pul_glo_z","l07_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 if (sqrt(match->localPositionError().yy())<0.1) {
	   makeFillHisto1D<TH1F,double>("l07p_res_loc_x","l07p_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l07p_pul_loc_x","l07p_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l07p_res_loc_y","l07p_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l07p_pul_loc_y","l07p_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l07p_res_glo_x","l07p_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l07p_pul_glo_x","l07p_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l07p_res_glo_y","l07p_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l07p_pul_glo_y","l07p_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l07p_res_glo_z","l07p_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l07p_pul_glo_z","l07p_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 } else {
	   makeFillHisto1D<TH1F,double>("l07s_res_loc_x","l07s_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l07s_pul_loc_x","l07s_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l07s_res_loc_y","l07s_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l07s_pul_loc_y","l07s_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l07s_res_glo_x","l07s_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l07s_pul_glo_x","l07s_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l07s_res_glo_y","l07s_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l07s_pul_glo_y","l07s_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l07s_res_glo_z","l07s_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l07s_pul_glo_z","l07s_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 }
       } else if (PXBDetId(hitId).layer()==8) {
	 makeFillHisto1D<TH1F,double>("l08_res_loc_x","l08_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l08_pul_loc_x","l08_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l08_res_loc_y","l08_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l08_pul_loc_y","l08_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l08_res_glo_x","l08_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l08_pul_glo_x","l08_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l08_res_glo_y","l08_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l08_pul_glo_y","l08_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l08_res_glo_z","l08_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l08_pul_glo_z","l08_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 if (sqrt(match->localPositionError().yy())<0.1) {
	   /*
	     makeFillHisto1D<TH1F,double>("l08p_res_loc_x","l08p_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	     makeFillHisto1D<TH1F,double>("l08p_pul_loc_x","l08p_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	     makeFillHisto1D<TH1F,double>("l08p_res_loc_y","l08p_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	     makeFillHisto1D<TH1F,double>("l08p_pul_loc_y","l08p_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	     makeFillHisto1D<TH1F,double>("l08p_res_glo_x","l08p_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	     makeFillHisto1D<TH1F,double>("l08p_pul_glo_x","l08p_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	     makeFillHisto1D<TH1F,double>("l08p_res_glo_y","l08p_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	     makeFillHisto1D<TH1F,double>("l08p_pul_glo_y","l08p_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	     makeFillHisto1D<TH1F,double>("l08p_res_glo_z","l08p_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	     makeFillHisto1D<TH1F,double>("l08p_pul_glo_z","l08p_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	   */
	 } else {
	   makeFillHisto1D<TH1F,double>("l08s_res_loc_x","l08s_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l08s_pul_loc_x","l08s_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l08s_res_loc_y","l08s_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l08s_pul_loc_y","l08s_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l08s_res_glo_x","l08s_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l08s_pul_glo_x","l08s_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l08s_res_glo_y","l08s_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l08s_pul_glo_y","l08s_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l08s_res_glo_z","l08s_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l08s_pul_glo_z","l08s_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 }
       } else if (PXBDetId(hitId).layer()==9) {
	 makeFillHisto1D<TH1F,double>("l09_res_loc_x","l09_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l09_pul_loc_x","l09_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l09_res_loc_y","l09_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l09_pul_loc_y","l09_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l09_res_glo_x","l09_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l09_pul_glo_x","l09_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l09_res_glo_y","l09_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l09_pul_glo_y","l09_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l09_res_glo_z","l09_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l09_pul_glo_z","l09_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 if (sqrt(match->localPositionError().yy())<0.1) {
	   /*
	     makeFillHisto1D<TH1F,double>("l09p_res_loc_x","l09p_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	     makeFillHisto1D<TH1F,double>("l09p_pul_loc_x","l09p_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	     makeFillHisto1D<TH1F,double>("l09p_res_loc_y","l09p_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	     makeFillHisto1D<TH1F,double>("l09p_pul_loc_y","l09p_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	     makeFillHisto1D<TH1F,double>("l09p_res_glo_x","l09p_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	     makeFillHisto1D<TH1F,double>("l09p_pul_glo_x","l09p_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	     makeFillHisto1D<TH1F,double>("l09p_res_glo_y","l09p_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	     makeFillHisto1D<TH1F,double>("l09p_pul_glo_y","l09p_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	     makeFillHisto1D<TH1F,double>("l09p_res_glo_z","l09p_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	     makeFillHisto1D<TH1F,double>("l09p_pul_glo_z","l09p_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	   */
	 } else {
	   makeFillHisto1D<TH1F,double>("l09s_res_loc_x","l09s_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l09s_pul_loc_x","l09s_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l09s_res_loc_y","l09s_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l09s_pul_loc_y","l09s_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l09s_res_glo_x","l09s_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l09s_pul_glo_x","l09s_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l09s_res_glo_y","l09s_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l09s_pul_glo_y","l09s_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l09s_res_glo_z","l09s_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l09s_pul_glo_z","l09s_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 }
       } else if (PXBDetId(hitId).layer()==10) {
	 makeFillHisto1D<TH1F,double>("l10_res_loc_x","l10_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	 makeFillHisto1D<TH1F,double>("l10_pul_loc_x","l10_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	 makeFillHisto1D<TH1F,double>("l10_res_loc_y","l10_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	 makeFillHisto1D<TH1F,double>("l10_pul_loc_y","l10_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	 makeFillHisto1D<TH1F,double>("l10_res_glo_x","l10_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	 makeFillHisto1D<TH1F,double>("l10_pul_glo_x","l10_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	 makeFillHisto1D<TH1F,double>("l10_res_glo_y","l10_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	 makeFillHisto1D<TH1F,double>("l10_pul_glo_y","l10_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	 makeFillHisto1D<TH1F,double>("l10_res_glo_z","l10_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	 makeFillHisto1D<TH1F,double>("l10_pul_glo_z","l10_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 if (sqrt(match->localPositionError().yy())<0.1) {
	   /*
	     makeFillHisto1D<TH1F,double>("l10p_res_loc_x","l10p_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	     makeFillHisto1D<TH1F,double>("l10p_pul_loc_x","l10p_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	     makeFillHisto1D<TH1F,double>("l10p_res_loc_y","l10p_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	     makeFillHisto1D<TH1F,double>("l10p_pul_loc_y","l10p_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	     makeFillHisto1D<TH1F,double>("l10p_res_glo_x","l10p_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	     makeFillHisto1D<TH1F,double>("l10p_pul_glo_x","l10p_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	     makeFillHisto1D<TH1F,double>("l10p_res_glo_y","l10p_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	     makeFillHisto1D<TH1F,double>("l10p_pul_glo_y","l10p_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	     makeFillHisto1D<TH1F,double>("l10p_res_glo_z","l10p_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	     makeFillHisto1D<TH1F,double>("l10p_pul_glo_z","l10p_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	   */
	 } else {
	   makeFillHisto1D<TH1F,double>("l10s_res_loc_x","l10s_res_loc_x",100,-0.01,0.01,match->localPosition().x()-simHitLP.x());
	   makeFillHisto1D<TH1F,double>("l10s_pul_loc_x","l10s_pul_loc_x",100,-5.,5.,(match->localPosition().x()-simHitLP.x())/sqrt(match->localPositionError().xx()));
	   makeFillHisto1D<TH1F,double>("l10s_res_loc_y","l10s_res_loc_y",100,-0.01,0.01,match->localPosition().y()-simHitLP.y());
	   makeFillHisto1D<TH1F,double>("l10s_pul_loc_y","l10s_pul_loc_y",100,-5.,5.,(match->localPosition().y()-simHitLP.y())/sqrt(match->localPositionError().yy()));
	   makeFillHisto1D<TH1F,double>("l10s_res_glo_x","l10s_res_glo_x",100,-0.01,0.01,ttrh->globalPosition().x()-simHitGP.x());
	   makeFillHisto1D<TH1F,double>("l10s_pul_glo_x","l10s_pul_glo_x",100,-5.,5.,(ttrh->globalPosition().x()-simHitGP.x())/sqrt(ttrh->globalPositionError().cxx()));
	   makeFillHisto1D<TH1F,double>("l10s_res_glo_y","l10s_res_glo_y",100,-0.01,0.01,ttrh->globalPosition().y()-simHitGP.y());
	   makeFillHisto1D<TH1F,double>("l10s_pul_glo_y","l10s_pul_glo_y",100,-5.,5.,(ttrh->globalPosition().y()-simHitGP.y())/sqrt(ttrh->globalPositionError().cyy()));
	   makeFillHisto1D<TH1F,double>("l10s_res_glo_z","l10s_res_glo_z",100,-0.1,0.1,ttrh->globalPosition().z()-simHitGP.z());
	   makeFillHisto1D<TH1F,double>("l10s_pul_glo_z","l10s_pul_glo_z",100,-5.,5.,(ttrh->globalPosition().z()-simHitGP.z())/sqrt(ttrh->globalPositionError().czz()));
	 }
       } 
     }

   }

   Handle<TrackCollection> pIn;
   //InputTag tag("initialStepTracks","","RECO2");
   InputTag tag("generalTracks");
   iEvent.getByLabel(tag,pIn);
   for (auto it=pIn->begin();it<pIn->end();++it) {
     cout << "track pt=" << it->pt()
	  << " eta="  <<  it->eta() 
	  << " hits="  <<  it->found() 
	  << " layers=" <<  it->hitPattern().trackerLayersWithMeasurement() 
	  << " pixlay=" <<  it->hitPattern().pixelLayersWithMeasurement() 
	  << " pi=" << sqrt(it->innerMomentum().mag2()) 
	  << " po=" << sqrt(it->outerMomentum().mag2()) 
	  << endl;
     makeFillHisto1D<TH1F,double>("layers","layers",20,0.,20.,it->hitPattern().trackerLayersWithMeasurement());
     makeFillHisto1D<TH1F,double>("hits","hits",20,0.,20.,it->found());
     makeFillHisto1D<TH1F,double>("dp_reco","dp_reco",1000,0.,2.,sqrt(it->innerMomentum().mag2())-sqrt(it->outerMomentum().mag2()));
     int nhit = 0;
     for (auto i=it->recHitsBegin(); i!=it->recHitsEnd(); i++){
       cout << "hit #" << nhit;
       TransientTrackingRecHit::RecHitPointer hit=builder->build(&**i );
       DetId hitId = hit->geographicalId();       
       if(hitId.det() == DetId::Tracker) {
	 /*if (hitId.subdetId() == StripSubdetector::TIB )  
	   cout << " - TIB " << TIBDetId(hitId).layer();
	 else if (hitId.subdetId() == StripSubdetector::TOB ) 
	   cout << " - TOB " << TOBDetId(hitId).layer();
	 else if (hitId.subdetId() == StripSubdetector::TEC ) 
	   cout << " - TEC " << TECDetId(hitId).wheel();
	 else if (hitId.subdetId() == StripSubdetector::TID ) 
	   cout << " - TID " << TIDDetId(hitId).wheel();
	 else if (hitId.subdetId() == StripSubdetector::TID ) 
	   cout << " - TID " << TIDDetId(hitId).wheel();
	   else*/ if (hitId.subdetId() == (int) PixelSubdetector::PixelBarrel ) 
	   cout << " - PixBar " << PXBDetId(hitId).layer();
	 else if (hitId.subdetId() == (int) PixelSubdetector::PixelEndcap )
	   cout << " - PixFwd " << PXFDetId(hitId).disk();
	 else 
	   cout << " UNKNOWN TRACKER HIT TYPE ";
       }
       if (hit->isValid()) cout << " id=" << hitId.rawId() << " gp=" << hit->globalPosition() << " r=" << hit->globalPosition().perp() 
				<<  " rl=" << geom->idToDet(hit->geographicalId())->surface().mediumProperties().radLen() 
				<<  " xi=" << geom->idToDet(hit->geographicalId())->surface().mediumProperties().xi() 
				<< endl;
       else cout << " id=" << hitId.rawId() << " - invalid hit" << endl;
       nhit++;
     }    

   }

}


// ------------ method called once each job just before starting event loop  ------------
void 
TestHits::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TestHits::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
TestHits::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
TestHits::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
TestHits::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
TestHits::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TestHits::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestHits);
