#include "Track.h"
#include "TFile.h"
#include "TTree.h"

struct LayerNumberConverter {
public:
  LayerNumberConverter() {}
  int convertLayerNumber(int cmsswdet, int cmsswlay, bool useMatched, int isStereo) {
    if (cmsswdet==2 || cmsswdet==4 || cmsswdet==6) return -1;
    if (cmsswdet==1) return cmsswlay-1;
    if (useMatched) {
      if (cmsswdet==3 && cmsswlay==1 && isStereo==-1) return 3;
      if (cmsswdet==3 && cmsswlay==2 && isStereo==-1) return 4;
      if (cmsswdet==3 && cmsswlay==3 && isStereo==0 ) return 5;
      if (cmsswdet==3 && cmsswlay==4 && isStereo==0 ) return 6;
      if (cmsswdet==5 && cmsswlay==1 && isStereo==-1) return 7;
      if (cmsswdet==5 && cmsswlay==2 && isStereo==-1) return 8;
      if (cmsswdet==5 && cmsswlay==3 && isStereo==0 ) return 9;
      if (cmsswdet==5 && cmsswlay==4 && isStereo==0 ) return 10;
      if (cmsswdet==5 && cmsswlay==5 && isStereo==0 ) return 11;
      if (cmsswdet==5 && cmsswlay==6 && isStereo==0 ) return 12;
      return -1;
    } else {
      if (cmsswdet==3 && cmsswlay==1 && isStereo==0) return 3;
      if (cmsswdet==3 && cmsswlay==1 && isStereo==1) return 4;
      if (cmsswdet==3 && cmsswlay==2 && isStereo==0) return 5;
      if (cmsswdet==3 && cmsswlay==2 && isStereo==1) return 6;
      if (cmsswdet==3 && cmsswlay==3 && isStereo==0) return 7;
      if (cmsswdet==3 && cmsswlay==4 && isStereo==0) return 8;
      if (cmsswdet==5 && cmsswlay==1 && isStereo==1) return 9;
      if (cmsswdet==5 && cmsswlay==1 && isStereo==0) return 10;
      if (cmsswdet==5 && cmsswlay==2 && isStereo==1) return 11;
      if (cmsswdet==5 && cmsswlay==2 && isStereo==0) return 12;
      if (cmsswdet==5 && cmsswlay==3 && isStereo==0) return 13;
      if (cmsswdet==5 && cmsswlay==4 && isStereo==0) return 14;
      if (cmsswdet==5 && cmsswlay==5 && isStereo==0) return 15;
      if (cmsswdet==5 && cmsswlay==6 && isStereo==0) return 16;
      return -1;
    }
  }  
  int convertDiskNumber(int cmsswdet, int cmsswdisk, bool useMatched, int isStereo) {
    if (cmsswdet==1 || cmsswdet==3 || cmsswdet==5) return -1;
    if (cmsswdet==2) return cmsswdisk-1;
    if (useMatched) {
      return -1;
    } else {
      if (cmsswdet==4 && cmsswdisk==1 && isStereo==0) return 2;
      if (cmsswdet==4 && cmsswdisk==1 && isStereo==1) return 3;
      if (cmsswdet==4 && cmsswdisk==2 && isStereo==0) return 4;
      if (cmsswdet==4 && cmsswdisk==2 && isStereo==1) return 5;
      if (cmsswdet==4 && cmsswdisk==3 && isStereo==0) return 6;
      if (cmsswdet==4 && cmsswdisk==3 && isStereo==1) return 7;
      if (cmsswdet==6 && cmsswdisk==1 && isStereo==1) return 8;
      if (cmsswdet==6 && cmsswdisk==1 && isStereo==0) return 9;
      if (cmsswdet==6 && cmsswdisk==2 && isStereo==1) return 10;
      if (cmsswdet==6 && cmsswdisk==2 && isStereo==0) return 11;
      if (cmsswdet==6 && cmsswdisk==3 && isStereo==1) return 12;
      if (cmsswdet==6 && cmsswdisk==3 && isStereo==0) return 13;
      if (cmsswdet==6 && cmsswdisk==4 && isStereo==1) return 14;
      if (cmsswdet==6 && cmsswdisk==4 && isStereo==0) return 15;
      if (cmsswdet==6 && cmsswdisk==5 && isStereo==1) return 16;
      if (cmsswdet==6 && cmsswdisk==5 && isStereo==0) return 17;
      if (cmsswdet==6 && cmsswdisk==6 && isStereo==1) return 18;
      if (cmsswdet==6 && cmsswdisk==6 && isStereo==0) return 19;
      if (cmsswdet==6 && cmsswdisk==7 && isStereo==1) return 20;
      if (cmsswdet==6 && cmsswdisk==7 && isStereo==0) return 21;
      if (cmsswdet==6 && cmsswdisk==8 && isStereo==1) return 22;
      if (cmsswdet==6 && cmsswdisk==8 && isStereo==0) return 23;
      if (cmsswdet==6 && cmsswdisk==9 && isStereo==1) return 24;
      if (cmsswdet==6 && cmsswdisk==9 && isStereo==0) return 25;
      return -1;
    }
  }
};

bool useMatched = false;
bool doEndcap = true;//otherwise do barrel, both at the same time not supported for now

int main() {

  using namespace std;

  LayerNumberConverter lnc;

  long long maxevt = 0;

  int nstot = 0;
  int nhitstot[Config::nLayers];
  std::fill_n(nhitstot, Config::nLayers, 0);

  TString outfilename = "";

  // TFile* f = TFile::Open("./ntuple_test_1GeV_10k_split.root"); maxevt = 3000;outfilename = "cmssw_3kxSingleMu1GeV_split_endcap.bin";
  TFile* f = TFile::Open(getenv("TKFILE_IN")); maxevt = 30000;outfilename = getenv("TKFILE_OUT");
  std::cout<<"I/O setup: TKFILE_IN "<<getenv("TKFILE_IN")<<" TKFILE_OUT "<<getenv("TKFILE_OUT")<<std::endl;
  //TFile* f = TFile::Open("./ntuple_test_1GeV_10k.root");
  // TFile* f = TFile::Open(useMatched ? "./ntuple_test_1GeV_10k_noSplit_mock_noFWD.root" : "./ntuple_test_1GeV_10k_split_mock_noFWD.root");maxevt = 3000;outfilename = (useMatched ? "cmssw_3kxSingleMu1GeV_polar_noSplit_mock_noFWD.bin" : "cmssw_3kxSingleMu1GeV_polar_split_mock_noFWD.bin");
  // TFile* f = TFile::Open(useMatched ? "./ntuple_test_10GeV_10k_noSplit_mock_noFWD.root" : "./ntuple_test_10GeV_10k_split_mock_noFWD.root");maxevt = 3000;outfilename = (useMatched ? "cmssw_3kxSingleMu10GeV_polar_noSplit_mock_noFWD.bin" : "cmssw_3kxSingleMu10GeV_polar_split_mock_noFWD.bin");
  // TFile* f = TFile::Open("./ntuple_test_ZTT_split_mock_noFWD.root");assert(useMatched==false);maxevt = 500;outfilename = "cmssw_500xZTT_polar_split_mock_noFWD.bin";
  // TFile* f = TFile::Open("./ntuple_test_TTbar_split_mock_noFWD.root");assert(useMatched==false);maxevt = 500;outfilename = "cmssw_500xTTbar_polar_split_mock_noFWD.bin";
  // TFile* f = TFile::Open("./ntuple_test_TTbarPU10_split_mock_noFWD.root");assert(useMatched==false);maxevt = 100;outfilename = "cmssw_100xTTbarPU10_polar_split_mock_noFWD.bin";
  // TFile* f = TFile::Open("./ntuple_test_TTbarPU35_split_mock_noFWD.root");assert(useMatched==false);maxevt = 100;outfilename = "cmssw_100xTTbarPU35_polar_split_mock_noFWD.bin";
  
  TTree* t = (TTree*) f->Get("trkTree/tree");

  FILE * fp;
  fp = fopen (outfilename.Data(), "wb");

  int evt;
  t->SetBranchAddress("evt",&evt);

  //sim tracks
  std::vector<float>* sim_eta = 0;
  std::vector<float>* sim_px = 0;
  std::vector<float>* sim_py = 0;
  std::vector<float>* sim_pz = 0;
  std::vector<float>* sim_prodx = 0;
  std::vector<float>* sim_prody = 0;
  std::vector<float>* sim_prodz = 0;
  std::vector<int>*   sim_q = 0;
  std::vector<std::vector<int> >* sim_pixelIdx = 0;
  std::vector<std::vector<int> >* sim_stripIdx = 0;
  std::vector<int>*   sim_trkIdx = 0;
  t->SetBranchAddress("sim_eta",&sim_eta);
  t->SetBranchAddress("sim_px",&sim_px);
  t->SetBranchAddress("sim_py",&sim_py);
  t->SetBranchAddress("sim_pz",&sim_pz);
  t->SetBranchAddress("sim_prodx",&sim_prodx);
  t->SetBranchAddress("sim_prody",&sim_prody);
  t->SetBranchAddress("sim_prodz",&sim_prodz);
  t->SetBranchAddress("sim_q",&sim_q);
  t->SetBranchAddress("sim_pixelIdx",&sim_pixelIdx);
  t->SetBranchAddress("sim_stripIdx",&sim_stripIdx);
  t->SetBranchAddress("sim_trkIdx",&sim_trkIdx);

  //rec tracks
  std::vector<int>*   trk_nValid = 0;
  std::vector<int>*   trk_nInvalid = 0;
  std::vector<int>*   trk_nLay = 0;
  std::vector<int>*   trk_seedIdx = 0;
  std::vector<std::vector<int> >* trk_pixelIdx = 0;
  std::vector<std::vector<int> >* trk_stripIdx = 0;
  std::vector<std::vector<int> >* trk_gluedIdx = 0;
  t->SetBranchAddress("trk_nValid",&trk_nValid);
  t->SetBranchAddress("trk_nInvalid",&trk_nInvalid);
  t->SetBranchAddress("trk_nLay",&trk_nLay);
  t->SetBranchAddress("trk_seedIdx",&trk_seedIdx);
  t->SetBranchAddress("trk_pixelIdx",&trk_pixelIdx);
  t->SetBranchAddress("trk_stripIdx",&trk_stripIdx);
  t->SetBranchAddress("trk_gluedIdx",&trk_gluedIdx);

  //seeds
  std::vector<float>*   see_x = 0;
  std::vector<float>*   see_y = 0;
  std::vector<float>*   see_z = 0;
  std::vector<float>*   see_px = 0;
  std::vector<float>*   see_py = 0;
  std::vector<float>*   see_pz = 0;
  std::vector<float>*   see_eta= 0;
  std::vector<float>*   see_pt = 0;
  std::vector<float>*   see_cov00 = 0;
  std::vector<float>*   see_cov01 = 0;
  std::vector<float>*   see_cov02 = 0;
  std::vector<float>*   see_cov03 = 0;
  std::vector<float>*   see_cov04 = 0;
  std::vector<float>*   see_cov05 = 0;
  std::vector<float>*   see_cov11 = 0;
  std::vector<float>*   see_cov12 = 0;
  std::vector<float>*   see_cov13 = 0;
  std::vector<float>*   see_cov14 = 0;
  std::vector<float>*   see_cov15 = 0;
  std::vector<float>*   see_cov22 = 0;
  std::vector<float>*   see_cov23 = 0;
  std::vector<float>*   see_cov24 = 0;
  std::vector<float>*   see_cov25 = 0;
  std::vector<float>*   see_cov33 = 0;
  std::vector<float>*   see_cov34 = 0;
  std::vector<float>*   see_cov35 = 0;
  std::vector<float>*   see_cov44 = 0;
  std::vector<float>*   see_cov45 = 0;
  std::vector<float>*   see_cov55 = 0;
  std::vector<int>*     see_q = 0;
  std::vector<int>*     see_algo = 0;
  std::vector<std::vector<int> >* see_pixelIdx = 0;
  t->SetBranchAddress("see_x",&see_x);
  t->SetBranchAddress("see_y",&see_y);
  t->SetBranchAddress("see_z",&see_z);
  t->SetBranchAddress("see_px",&see_px);
  t->SetBranchAddress("see_py",&see_py);
  t->SetBranchAddress("see_pz",&see_pz);
  t->SetBranchAddress("see_eta",&see_eta);
  t->SetBranchAddress("see_pt",&see_pt);
  t->SetBranchAddress("see_cov00",&see_cov00);
  t->SetBranchAddress("see_cov01",&see_cov01);
  t->SetBranchAddress("see_cov02",&see_cov02);
  t->SetBranchAddress("see_cov03",&see_cov03);
  t->SetBranchAddress("see_cov04",&see_cov04);
  t->SetBranchAddress("see_cov05",&see_cov05);
  t->SetBranchAddress("see_cov11",&see_cov11);
  t->SetBranchAddress("see_cov12",&see_cov12);
  t->SetBranchAddress("see_cov13",&see_cov13);
  t->SetBranchAddress("see_cov14",&see_cov14);
  t->SetBranchAddress("see_cov15",&see_cov15);
  t->SetBranchAddress("see_cov22",&see_cov22);
  t->SetBranchAddress("see_cov23",&see_cov23);
  t->SetBranchAddress("see_cov24",&see_cov24);
  t->SetBranchAddress("see_cov25",&see_cov25);
  t->SetBranchAddress("see_cov33",&see_cov33);
  t->SetBranchAddress("see_cov34",&see_cov34);
  t->SetBranchAddress("see_cov35",&see_cov35);
  t->SetBranchAddress("see_cov44",&see_cov44);
  t->SetBranchAddress("see_cov45",&see_cov45);
  t->SetBranchAddress("see_cov55",&see_cov55);
  t->SetBranchAddress("see_q",&see_q);
  t->SetBranchAddress("see_algo",&see_algo);
  t->SetBranchAddress("see_pixelIdx",&see_pixelIdx);

  //pixel hits
  vector<int>*    pix_isBarrel = 0;
  vector<int>*    pix_lay = 0;
  vector<int>*    pix_simTrkIdx = 0;
  vector<int>*    pix_particle = 0;
  vector<int>*    pix_process = 0;
  vector<int>*    pix_posFromTrack = 0;
  vector<int>*    pix_onTrack = 0;
  vector<float>*  pix_x = 0;
  vector<float>*  pix_y = 0;
  vector<float>*  pix_z = 0;
  vector<float>*  pix_xx = 0;
  vector<float>*  pix_xy = 0;
  vector<float>*  pix_yy = 0;
  vector<float>*  pix_yz = 0;
  vector<float>*  pix_zz = 0;
  vector<float>*  pix_zx = 0;
  t->SetBranchAddress("pix_isBarrel",&pix_isBarrel);
  t->SetBranchAddress("pix_lay",&pix_lay);
  t->SetBranchAddress("pix_simTrkIdx",&pix_simTrkIdx);
  t->SetBranchAddress("pix_particle",&pix_particle);
  t->SetBranchAddress("pix_process",&pix_process);
  t->SetBranchAddress("pix_posFromTrack",&pix_posFromTrack);
  t->SetBranchAddress("pix_onTrack",&pix_onTrack);
  t->SetBranchAddress("pix_x",&pix_x);
  t->SetBranchAddress("pix_y",&pix_y);
  t->SetBranchAddress("pix_z",&pix_z);
  t->SetBranchAddress("pix_xx",&pix_xx);
  t->SetBranchAddress("pix_xy",&pix_xy);
  t->SetBranchAddress("pix_yy",&pix_yy);
  t->SetBranchAddress("pix_yz",&pix_yz);
  t->SetBranchAddress("pix_zz",&pix_zz);
  t->SetBranchAddress("pix_zx",&pix_zx);

  //strip hits
  vector<int>*    glu_monoIdx = 0;
  vector<int>*    glu_stereoIdx = 0;
  vector<int>*    glu_isBarrel = 0;
  vector<int>*    glu_lay = 0;
  vector<int>*    glu_det = 0;
  vector<int>*    glu_posFromTrack = 0;
  vector<int>*    glu_onTrack = 0;
  vector<float>*  glu_x = 0;
  vector<float>*  glu_y = 0;
  vector<float>*  glu_z = 0;
  vector<float>*  glu_xx = 0;
  vector<float>*  glu_xy = 0;
  vector<float>*  glu_yy = 0;
  vector<float>*  glu_yz = 0;
  vector<float>*  glu_zz = 0;
  vector<float>*  glu_zx = 0;
  if (useMatched) {
    t->SetBranchAddress("glu_monoIdx",&glu_monoIdx);
    t->SetBranchAddress("glu_stereoIdx",&glu_stereoIdx);
    t->SetBranchAddress("glu_isBarrel",&glu_isBarrel);
    t->SetBranchAddress("glu_lay",&glu_lay);
    t->SetBranchAddress("glu_det",&glu_det);
    t->SetBranchAddress("glu_posFromTrack",&glu_posFromTrack);
    t->SetBranchAddress("glu_onTrack",&glu_onTrack);
    t->SetBranchAddress("glu_x",&glu_x);
    t->SetBranchAddress("glu_y",&glu_y);
    t->SetBranchAddress("glu_z",&glu_z);
    t->SetBranchAddress("glu_xx",&glu_xx);
    t->SetBranchAddress("glu_xy",&glu_xy);
    t->SetBranchAddress("glu_yy",&glu_yy);
    t->SetBranchAddress("glu_yz",&glu_yz);
    t->SetBranchAddress("glu_zz",&glu_zz);
    t->SetBranchAddress("glu_zx",&glu_zx);
  }
  vector<int>*    str_isBarrel = 0;
  vector<int>*    str_isStereo = 0;
  vector<int>*    str_lay = 0;
  vector<int>*    str_det = 0;
  vector<int>*    str_simTrkIdx = 0;
  vector<int>*    str_particle = 0;
  vector<int>*    str_process = 0;
  vector<int>*    str_posFromTrack = 0;
  vector<int>*    str_onTrack = 0;
  vector<float>*  str_x = 0;
  vector<float>*  str_y = 0;
  vector<float>*  str_z = 0;
  vector<float>*  str_xx = 0;
  vector<float>*  str_xy = 0;
  vector<float>*  str_yy = 0;
  vector<float>*  str_yz = 0;
  vector<float>*  str_zz = 0;
  vector<float>*  str_zx = 0;
  t->SetBranchAddress("str_isBarrel",&str_isBarrel);
  t->SetBranchAddress("str_isStereo",&str_isStereo);
  t->SetBranchAddress("str_lay",&str_lay);
  t->SetBranchAddress("str_det",&str_det);
  t->SetBranchAddress("str_simTrkIdx",&str_simTrkIdx);
  t->SetBranchAddress("str_particle",&str_particle);
  t->SetBranchAddress("str_process",&str_process);
  t->SetBranchAddress("str_posFromTrack",&str_posFromTrack);
  t->SetBranchAddress("str_onTrack",&str_onTrack);
  t->SetBranchAddress("str_x",&str_x);
  t->SetBranchAddress("str_y",&str_y);
  t->SetBranchAddress("str_z",&str_z);
  t->SetBranchAddress("str_xx",&str_xx);
  t->SetBranchAddress("str_xy",&str_xy);
  t->SetBranchAddress("str_yy",&str_yy);
  t->SetBranchAddress("str_yz",&str_yz);
  t->SetBranchAddress("str_zz",&str_zz);
  t->SetBranchAddress("str_zx",&str_zx);

  fwrite(&maxevt, sizeof(int), 1, fp);

  int nlPrinted = 0;
  long long totentries = t->GetEntriesFast();

  long long savedEvents = 0;
  for (long long i = 0; savedEvents < maxevt && i<totentries; ++i) {

    cout << "process entry i=" << i << " out of " << totentries << ", saved so far " << savedEvents << ", with max=" << maxevt << endl;

    t->GetEntry(i);

    cout << "edm event=" << evt << endl;

    if (sim_q->size()==0) {
      cout << "branches not loaded" << endl; exit(1);
    }
    
    vector<Track> simTracks_;
    vector<int> simTrackIdx_(sim_q->size(),-1);//keep track of original index in ntuple
    vector<int> seedSimIdx(see_x->size(),-1);
    for (int isim = 0; isim < sim_q->size(); ++isim) {

      //if (fabs(sim_eta->at(isim))>0.8) continue;

      int trkIdx = sim_trkIdx->at(isim);
      //if (trkIdx<0) continue;

      int nlay = 0;
      int hitlay[Config::nLayers];
      std::fill_n(hitlay, Config::nLayers, 0);
      if (trkIdx>=0) {
	for (int ihit = 0; ihit < trk_pixelIdx->at(trkIdx).size(); ++ihit) {
	  int ipix = trk_pixelIdx->at(trkIdx).at(ihit);
	  if (ipix<0) continue;
	  if (doEndcap) {
	    int cmsswdisk = lnc.convertDiskNumber((pix_isBarrel->at(ipix)?1:2),pix_lay->at(ipix),useMatched,-1);
	    if (cmsswdisk>=0 && cmsswdisk<Config::nLayers) hitlay[cmsswdisk]++;
	  } else {
	    int cmsswlay = lnc.convertLayerNumber((pix_isBarrel->at(ipix)?1:2),pix_lay->at(ipix),useMatched,-1);
	    if (cmsswlay>=0 && cmsswlay<Config::nLayers) hitlay[cmsswlay]++;
	  }
	}
	if (useMatched) {
	  for (int ihit = 0; ihit < trk_gluedIdx->at(trkIdx).size(); ++ihit) {
	    int iglu = trk_gluedIdx->at(trkIdx).at(ihit);
	    if (iglu<0) continue;
	    int cmsswlay = lnc.convertLayerNumber(glu_det->at(iglu),glu_lay->at(iglu),useMatched,-1);
	    if (cmsswlay>=0 && cmsswlay<Config::nLayers) hitlay[cmsswlay]++;
	  }
	}
	for (int ihit = 0; ihit < trk_stripIdx->at(trkIdx).size(); ++ihit) {
	  int istr = trk_stripIdx->at(trkIdx).at(ihit);
	  if (istr<0) continue;
	  if (doEndcap) {
	    int cmsswdisk = lnc.convertDiskNumber(str_det->at(istr),str_lay->at(istr),useMatched,str_isStereo->at(istr));
	    if (cmsswdisk>=0 && cmsswdisk<Config::nLayers) hitlay[cmsswdisk]++;
	  } else {
	    int cmsswlay = lnc.convertLayerNumber(str_det->at(istr),str_lay->at(istr),useMatched,str_isStereo->at(istr));
	    if (cmsswlay>=0 && cmsswlay<Config::nLayers) hitlay[cmsswlay]++;
	  }
	}
	for (int i=0;i<Config::nLayers;i++) if (hitlay[i]>0) nlay++;
      }

      //cout << Form("track q=%2i p=(%6.3f, %6.3f, %6.3f) x=(%6.3f, %6.3f, %6.3f) nlay=%i",sim_q->at(isim),sim_px->at(isim),sim_py->at(isim),sim_pz->at(isim),sim_prodx->at(isim),sim_prody->at(isim),sim_prodz->at(isim),nlay) << endl;

      SVector3 pos(sim_prodx->at(isim),sim_prody->at(isim),sim_prodz->at(isim));
      SVector3 mom(sim_px->at(isim),sim_py->at(isim),sim_pz->at(isim));
      SMatrixSym66 err;
      err.At(0,0) = sim_prodx->at(isim)*sim_prodx->at(isim);
      err.At(1,1) = sim_prody->at(isim)*sim_prody->at(isim);
      err.At(2,2) = sim_prodz->at(isim)*sim_prodz->at(isim);
      err.At(3,3) = sim_px->at(isim)*sim_px->at(isim);
      err.At(4,4) = sim_py->at(isim)*sim_py->at(isim);
      err.At(5,5) = sim_pz->at(isim)*sim_pz->at(isim);
      TrackState state(sim_q->at(isim), pos, mom, err);
#ifdef CCSCOORD
      //begin test CCS coordinates, define CCSCOORD in $(MICTESTDIR)/Config.h
      state.convertFromCartesianToCCS();
      //end test CCS coordinates
#endif
      int hitIdxArr[Config::nLayers];
      std::fill_n(hitIdxArr, Config::nLayers, -1);//this is set afterwards
      Track track(state, float(nlay), isim, Config::nLayers, hitIdxArr);//store number of reco hits in place of track chi2
      if (trkIdx>=0) {
	int seedIdx = trk_seedIdx->at(trkIdx);
	if (see_pixelIdx->at(seedIdx).size()!=0) {
	  seedSimIdx[seedIdx] = simTracks_.size();
	}
      }
      simTrackIdx_[isim] = simTracks_.size();
      simTracks_.push_back(track);      
    }

    if (simTracks_.size()==0) continue;
    //if (simTracks_.size()<2) continue;

    vector<Track> seedTracks_;
    vector<vector<int> > pixHitSeedIdx(pix_lay->size());
    for (int is = 0; is<see_x->size(); ++is) {
      if (see_algo->at(is)!=4) continue;//select seed in acceptance
      //if (see_pt->at(is)<0.5 || fabs(see_eta->at(is))>0.8) continue;//select seed in acceptance
      SVector3 pos = SVector3(see_x->at(is),see_y->at(is),see_z->at(is));
      SVector3 mom = SVector3(see_px->at(is),see_py->at(is),see_pz->at(is));
      SMatrixSym66 err;
      err.At(0,0) = see_cov00->at(is);
      err.At(0,1) = see_cov01->at(is);
      err.At(0,2) = see_cov02->at(is);
      err.At(0,3) = see_cov03->at(is);
      err.At(0,4) = see_cov04->at(is);
      err.At(0,5) = see_cov05->at(is);
      err.At(1,1) = see_cov11->at(is);
      err.At(1,2) = see_cov12->at(is);
      err.At(1,3) = see_cov13->at(is);
      err.At(1,4) = see_cov14->at(is);
      err.At(1,5) = see_cov15->at(is);
      err.At(2,2) = see_cov22->at(is);
      err.At(2,3) = see_cov23->at(is);
      err.At(2,4) = see_cov24->at(is);
      err.At(2,5) = see_cov25->at(is);
      err.At(3,3) = see_cov33->at(is);
      err.At(3,4) = see_cov34->at(is);
      err.At(3,5) = see_cov35->at(is);
      err.At(4,4) = see_cov44->at(is);
      err.At(4,5) = see_cov45->at(is);
      err.At(5,5) = see_cov55->at(is);
      TrackState state(see_q->at(is), pos, mom, err);
#ifdef CCSCOORD
      //begin test CCS coordinates
      state.convertFromCartesianToCCS();
      //end test CCS coordinates
#endif
      int hitIdxArr[Config::nLayers];
      std::fill_n(hitIdxArr, Config::nLayers, -1);//this is set afterwards
      //if (seedSimIdx[is]<0) continue;//FIXME, selecting only true seeds for now!
      Track track(state, 0, seedSimIdx[is], Config::nLayers, hitIdxArr);
      if (see_pixelIdx->at(is).size()!=3) continue;//only seeds with 3 pixel hits
      for (int ip=0; ip<see_pixelIdx->at(is).size(); ip++) {
	unsigned int ipix = see_pixelIdx->at(is)[ip];
	//cout << "ipix=" << ipix << " seed=" << seedTracks_.size() << endl;
	pixHitSeedIdx[ipix].push_back(seedTracks_.size());
      }
      seedTracks_.push_back(track);
    }

    if (seedTracks_.size()==0) continue;

    vector<vector<Hit> > layerHits_;
    vector<MCHitInfo> simHitsInfo_;
    int totHits = 0;
    layerHits_.resize(Config::nLayers);
    for (int ipix = 0; ipix < pix_lay->size(); ++ipix) {
      int ilay = -1;
      if (doEndcap) {
	if (pix_isBarrel->at(ipix)==1) continue;
	ilay = lnc.convertDiskNumber((pix_isBarrel->at(ipix)?1:2),pix_lay->at(ipix),useMatched,-1);
      } else {
	if (pix_isBarrel->at(ipix)==0) continue;
	ilay = lnc.convertLayerNumber((pix_isBarrel->at(ipix)?1:2),pix_lay->at(ipix),useMatched,-1);
      }
      if (ilay<0) continue;
      int simTkIdx = -1;
      if (pix_simTrkIdx->at(ipix)>=0) simTkIdx = simTrackIdx_[pix_simTrkIdx->at(ipix)];
      //cout << Form("pix lay=%i bar=%i x=(%6.3f, %6.3f, %6.3f)",ilay+1,pix_isBarrel->at(ipix),pix_x->at(ipix),pix_y->at(ipix),pix_z->at(ipix)) << endl;
      SVector3 pos(pix_x->at(ipix),pix_y->at(ipix),pix_z->at(ipix));
      SMatrixSym33 err;
      err.At(0,0) = pix_xx->at(ipix);
      err.At(1,1) = pix_yy->at(ipix);
      err.At(2,2) = pix_zz->at(ipix);
      err.At(0,1) = pix_xy->at(ipix);
      err.At(0,2) = pix_zx->at(ipix);
      err.At(1,2) = pix_yz->at(ipix);
      if (simTkIdx>=0) simTracks_[simTkIdx].setHitIdx(ilay, layerHits_[ilay].size());
      for (int is=0;is<pixHitSeedIdx[ipix].size();is++) {
	//cout << "xxx ipix=" << ipix << " seed=" << pixHitSeedIdx[ipix][is] << endl;
      	seedTracks_[pixHitSeedIdx[ipix][is]].setHitIdx(ilay, layerHits_[ilay].size());
      }
      Hit hit(pos, err, totHits);
      layerHits_[ilay].push_back(hit);
      MCHitInfo hitInfo(simTkIdx, ilay, layerHits_[ilay].size()-1);
      hitInfo.mcHitID_ = totHits;
      simHitsInfo_.push_back(hitInfo);
      totHits++;
    }

    if (useMatched) {
      for (int iglu = 0; iglu < glu_lay->size(); ++iglu) {
	if (glu_isBarrel->at(iglu)==0) continue;
	int simTkIdx = -1;
	if (str_simTrkIdx->at(glu_monoIdx->at(iglu))>=0 /*|| str_simTrkIdx->at(glu_stereoIdx->at(iglu))>=0*/) simTkIdx = simTrackIdx_[str_simTrkIdx->at(glu_monoIdx->at(iglu))];
	int ilay = lnc.convertLayerNumber(glu_det->at(iglu),glu_lay->at(iglu),useMatched,-1);
	//cout << ilay << " " << str_simTrkIdx->at(glu_monoIdx->at(iglu)) << " " << str_process->at(glu_monoIdx->at(iglu)) << " " << str_simTrkIdx->at(glu_stereoIdx->at(iglu)) << " " << str_process->at(glu_stereoIdx->at(iglu)) << endl;
	// cout << Form("glu lay=%i det=%i bar=%i x=(%6.3f, %6.3f, %6.3f)",ilay+1,glu_det->at(iglu),glu_isBarrel->at(iglu),glu_x->at(iglu),glu_y->at(iglu),glu_z->at(iglu)) << endl;
	SVector3 pos(glu_x->at(iglu),glu_y->at(iglu),glu_z->at(iglu));
	SMatrixSym33 err;
	err.At(0,0) = glu_xx->at(iglu);
	err.At(1,1) = glu_yy->at(iglu);
	err.At(2,2) = glu_zz->at(iglu);
	err.At(0,1) = glu_xy->at(iglu);
	err.At(0,2) = glu_zx->at(iglu);
	err.At(1,2) = glu_yz->at(iglu);	
	if (simTkIdx>=0) simTracks_[simTkIdx].setHitIdx(ilay, layerHits_[ilay].size());
	Hit hit(pos, err, totHits);
	layerHits_[ilay].push_back(hit);
	MCHitInfo hitInfo(simTkIdx, ilay, layerHits_[ilay].size()-1);
	hitInfo.mcHitID_ = totHits;
	simHitsInfo_.push_back(hitInfo);
	totHits++;
      }
    }

    vector<int> strIdx;
    strIdx.resize(str_lay->size());
    for (int istr = 0; istr < str_lay->size(); ++istr) {
      int ilay = -1;
      if (doEndcap) {
	if (str_isBarrel->at(istr)==1) continue;
	ilay = lnc.convertDiskNumber(str_det->at(istr),str_lay->at(istr),useMatched,str_isStereo->at(istr));
      } else {
	if (str_isBarrel->at(istr)==0) continue;
	ilay = lnc.convertLayerNumber(str_det->at(istr),str_lay->at(istr),useMatched,str_isStereo->at(istr));
	if (useMatched && str_isStereo->at(istr)) continue;
      }
      if (ilay==-1 || ilay>=Config::nLayers) continue;
      int simTkIdx = -1;
      if (str_simTrkIdx->at(istr)>=0) simTkIdx = simTrackIdx_[str_simTrkIdx->at(istr)];
      //if (str_onTrack->at(istr)==0) continue;//do not consider hits that are not on track!
      //cout << Form("str lay=%i istr=%i tridx=%i bar=%i x=(%6.3f, %6.3f, %6.3f) r=%6.3f proc=%i part=%i onTrk=%i isStereo=%i",ilay+1,istr,str_simTrkIdx->at(istr),str_isBarrel->at(istr),str_x->at(istr),str_y->at(istr),str_z->at(istr),sqrt(pow(str_x->at(istr),2)+pow(str_y->at(istr),2)),str_process->at(istr),str_particle->at(istr),str_onTrack->at(istr),str_isStereo->at(istr)) << endl;
      SVector3 pos(str_x->at(istr),str_y->at(istr),str_z->at(istr));
      SMatrixSym33 err;
      err.At(0,0) = str_xx->at(istr);
      err.At(1,1) = str_yy->at(istr);
      err.At(2,2) = str_zz->at(istr);
      err.At(0,1) = str_xy->at(istr);
      err.At(0,2) = str_zx->at(istr);
      err.At(1,2) = str_yz->at(istr);
      if (simTkIdx>=0) simTracks_[simTkIdx].setHitIdx(ilay, layerHits_[ilay].size());
      Hit hit(pos, err, totHits);
      layerHits_[ilay].push_back(hit);
      MCHitInfo hitInfo(simTkIdx, ilay, layerHits_[ilay].size()-1);
      hitInfo.mcHitID_ = totHits;
      simHitsInfo_.push_back(hitInfo);
      totHits++;
    }

    // bool allTracksAllHits = true;
    for (int i=0;i<simTracks_.size();++i) {
      simTracks_[i].setNGoodHitIdx();
      // if (simTracks_[i].nFoundHits()!=Config::nLayers) allTracksAllHits = false;
    }
    // if (!allTracksAllHits) continue;

    int nt = simTracks_.size();
    fwrite(&nt, sizeof(int), 1, fp);
    fwrite(&simTracks_[0], sizeof(Track), nt, fp);
    
    printf("number of simTracks %i\n",nt);

    int nl = layerHits_.size();
    fwrite(&nl, sizeof(int), 1, fp);
    for (int il = 0; il<nl; ++il) {
      int nh = layerHits_[il].size();
      fwrite(&nh, sizeof(int), 1, fp);
      fwrite(&layerHits_[il][0], sizeof(Hit), nh, fp);
      nhitstot[il]+=nh;
    }
    
    int nm = simHitsInfo_.size();
    fwrite(&nm, sizeof(int), 1, fp);
    fwrite(&simHitsInfo_[0], sizeof(MCHitInfo), nm, fp);

    int ns = seedTracks_.size();
    fwrite(&ns, sizeof(int), 1, fp);
    fwrite(&seedTracks_[0], sizeof(Track), ns, fp);
    nstot+=ns;

    printf("\n");

    if (savedEvents < 10 || nlPrinted < 10000000){//print at most 10 events or at most 10M lines
      for (int il = 0; il<nl; ++il) {
	int nh = layerHits_[il].size();
	for (int ih=0; ih<nh; ++ih ) {
	  printf("lay=%i idx=%i mcid=%i x=(%6.3f, %6.3f, %6.3f) r=%6.3f\n",il+1,ih,layerHits_[il][ih].mcHitID(),layerHits_[il][ih].x(),layerHits_[il][ih].y(),layerHits_[il][ih].z(),sqrt(pow(layerHits_[il][ih].x(),2)+pow(layerHits_[il][ih].y(),2)));
	  nlPrinted++;
	}
      }
      
      for (int i=0;i<nt;++i) {
	printf("sim track id=%i q=%2i p=(%6.3f, %6.3f, %6.3f) x=(%6.3f, %6.3f, %6.3f) pT=%7.4f nTotal=%i nFound=%i \n",i,simTracks_[i].charge(),simTracks_[i].px(),simTracks_[i].py(),simTracks_[i].pz(),simTracks_[i].x(),simTracks_[i].y(),simTracks_[i].z(),sqrt(pow(simTracks_[i].px(),2)+pow(simTracks_[i].py(),2)),simTracks_[i].nTotalHits(),simTracks_[i].nFoundHits());
	nlPrinted++;
	for (int ih=0;ih<Config::nLayers;++ih){
	  printf("track #%i hit #%i idx=%i\n",i,ih,simTracks_[i].getHitIdx(ih));
	  nlPrinted++;
	}
      }
      
      
      for (int i=0;i<ns;++i) {
	printf("seed id=%i label=%i q=%2i pT=%6.3f p=(%6.3f, %6.3f, %6.3f) x=(%6.3f, %6.3f, %6.3f)\n",i,seedTracks_[i].label(),seedTracks_[i].charge(),seedTracks_[i].pT(),seedTracks_[i].px(),seedTracks_[i].py(),seedTracks_[i].pz(),seedTracks_[i].x(),seedTracks_[i].y(),seedTracks_[i].z());
	nlPrinted++;
	for (int ih=0;ih<3;++ih){
	  printf("seed #%i hit #%i idx=%i\n",i,ih,seedTracks_[i].getHitIdx(ih));
	  nlPrinted++;
	}
      }
    }

    savedEvents++;
    printf("end of event %i\n",savedEvents);
  }

  printf("closing\n");
  fclose (fp);
  printf("\n saved %i events\n",savedEvents);

  printf("number of seeds %f\n",float(nstot)/float(savedEvents));
  for (int il=0;il<Config::nLayers;++il)
    printf("number of hits in layer %i = %f\n",il,float(nhitstot[il])/float(savedEvents));

}
