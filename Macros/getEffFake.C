/*
root -b -q getEffFake.C\(\"mtv.root\"\)
*/

void getEffFake(TString mtvfilename) {
  
  TFile* f = TFile::Open(mtvfilename);

  TH1F* nr = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_reco_eta");
  TH1F* nar = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_assoc(recoToSim)_eta");
  TH1F* ns = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_simul_eta");
  TH1F* nas = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_assoc(simToReco)_eta");
  TH1F* nd = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_duplicate_eta");


  cout << Form("Efficiency: %7.4f  -  FakeRate: %7.4f  -  DuplRate: %7.4f",100.*nas->Integral()/ns->Integral(),100.*(nr->Integral()-nar->Integral())/nr->Integral(),100*nd->Integral()/nr->Integral()) << endl;

  /*
  TH1F* nh = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/hits");
  TH1F* nah = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/assocSharedHit");
  cout << "nr->Integral()=" << nr->Integral() << endl;
  cout << "nar->Integral()=" << nar->Integral() << endl;
  cout << "ns->Integral()=" << ns->Integral() << endl;
  cout << "nas->Integral()=" << nas->Integral() << endl;
  cout << "nd->Integral()=" << nd->Integral() << endl;
  cout << "nh->GetEntries()=" << nh->GetEntries() << endl;
  cout << "nah->GetEntries()=" << nah->GetEntries() << endl;
  */

  TH1F* nsr = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_simul_vertpos");
  TH1F* nasr = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/num_assoc(simToReco)_vertpos");

  cout << Form("Efficiency: %7.4f  -  Prompt: %7.4f  -  Displaced: %7.4f",100.*nasr->Integral()/nsr->Integral(),100.*nasr->Integral(1,3)/nsr->Integral(1,3),100.*nasr->Integral(4,60)/nsr->Integral(4,60)) << endl;

}
