{

//   TFile *_file_old = TFile::Open("testHPtracks_default_3strip.root");
//   TFile *_file_new = TFile::Open("testHPtracks_tib1tib2-tob1.root");

//   TFile *_file_old = TFile::Open("testHPtracks_default_alleta.root");
//   TFile *_file_new = TFile::Open("testHPtracks_tib1tib2tib3_alleta_newfit_v2.root");

  TFile *_file_old = TFile::Open("testHPtracks_default_15eta19.root");
  //TFile *_file_new = TFile::Open("testHPtracks_tib1tib2tib3_15eta19_newfit_v2.root");
  TFile *_file_new = TFile::Open("testHPtracks_tib1tib2tib3_15eta19_mtec_newfit_v2.root");

  TCanvas c1;
  TH1F* seedhit_old = (TH1F*) _file_old->Get("demo/pixelLessStepSeeds_nHits_sd");
  TH1F* seedhit_new = (TH1F*) _file_new->Get("demo/pixelLessStepSeeds_nHits_sd");
  seedhit_old->SetLineColor(kBlack);
  seedhit_new->SetLineColor(kRed);
  seedhit_old->Draw("H");
  seedhit_new->Draw("HSAME");

  TCanvas c2;
  TH1F* eta_old = (TH1F*) _file_old->Get("demo/pixelLessStepTracksWithQuality_eta_hp");
  TH1F* eta_new = (TH1F*) _file_new->Get("demo/pixelLessStepTracksWithQuality_eta_hp");
  eta_old->SetLineColor(kBlack);
  eta_new->SetLineColor(kRed);
  eta_old->Draw("H");
  eta_new->Draw("HSAME");

  TCanvas c3;
  c3.SetLogx();
  TH1F* pt_old = (TH1F*) _file_old->Get("demo/pixelLessStepTracksWithQuality_pt_hp");
  TH1F* pt_new = (TH1F*) _file_new->Get("demo/pixelLessStepTracksWithQuality_pt_hp");
  pt_old->SetLineColor(kBlack);
  pt_new->SetLineColor(kRed);
  pt_old->Draw("H");
  pt_new->Draw("HSAME");

  TCanvas c;
  TH1F* pt_old = (TH1F*) _file_old->Get("demo/pixelLessStepTracksWithQuality_nlayer_tec_hp");
  TH1F* pt_new = (TH1F*) _file_new->Get("demo/pixelLessStepTracksWithQuality_nlayer_tec_hp");
  pt_old->SetLineColor(kBlack);
  pt_new->SetLineColor(kRed);
  pt_old->Draw("H");
  pt_new->Draw("HSAME");

}
