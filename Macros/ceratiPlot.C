{

  gROOT->Reset();
  gROOT->ProcessLine(".L tdrStyle.C");
  gROOT->ProcessLine("setTDRStyle()");
  gStyle->SetOptStat(0);

  bool dovsr = 1;
  bool dovspt = 0;

  if (dovsr==1&&dovspt==1) return;
  if (dovsr==0&&dovspt==0) return;

  TFile *f=0;
  TH1F *iter0=0,*iter1=0,*iter2=0,*iter3=0,*iter4=0,*iter5=0,*iter6=0,*iter9=0,*iter10=0,*iterA=0,*iterS=0;

  if (dovsr) { 
    //f = TFile::Open("mtv_20PU50ns_defVal_pt08-lip150-bin15-1k.root");
    f = TFile::Open("mtv_20PU50ns_defVal_pt05-lip30-bin15-1k.root");
    iterA = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter0 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoZeroHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter1 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoFirstHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter2 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoSecondHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter3 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoThirdHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter4 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoFourthHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter5 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoFifthHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter6 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoSixthHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter9 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoNinthHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter10 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoTenthHp_AssociatorByHitsRecoDenom/effic_vs_vertpos");
    iter0.Add(iter9);
    iter0.Add(iter10);

    iterN = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoHp_AssociatorByHitsRecoDenom/num_assoc(simToReco)_vertpos");
    iterS = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoHp_AssociatorByHitsRecoDenom/num_simul_vertpos");
    TCanvas cs;
    cs.SetLogy();
    iterS->Draw();
    cs.SaveAs("num_simul_vertpos.png");
    iterN->Draw();
    cs.SaveAs("num_assoc_simul_vertpos.png");
  }

  if (dovspt) {
    f = TFile::Open("mtv_20PU50ns_defVal_pt01tip20.root");
    iterA = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoHp_AssociatorByHitsRecoDenom//efficPt");
    iter0 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoZeroHp_AssociatorByHitsRecoDenom/efficPt");
    iter1 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoFirstHp_AssociatorByHitsRecoDenom/efficPt");
    iter2 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoSecondHp_AssociatorByHitsRecoDenom/efficPt");
    iter3 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoThirdHp_AssociatorByHitsRecoDenom/efficPt");
    iter4 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoFourthHp_AssociatorByHitsRecoDenom/efficPt");
    iter5 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoFifthHp_AssociatorByHitsRecoDenom/efficPt");
    iter6 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoSixthHp_AssociatorByHitsRecoDenom/efficPt");
    iter9 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoNinthHp_AssociatorByHitsRecoDenom/efficPt");
    iter10 = (TH1F*) f->Get("DQMData/Tracking/Track/cutsRecoTenthHp_AssociatorByHitsRecoDenom/efficPt");
    iter0.Add(iter9);
    iter0.Add(iter10);
 }

  iterA->SetMarkerColor(kBlack);
  iterA->SetFillColor(kBlack);
  iterA->SetFillStyle(3003);

  iter0->SetFillColor(kBlue-10);
  iter1->SetFillColor(kBlue-9);
  iter2->SetFillColor(kBlue-7);
  iter3->SetFillColor(kOrange+1);
  iter4->SetFillColor(kOrange+2);
  iter5->SetFillColor(kGreen+1);
  iter6->SetFillColor(kGreen+3);

  iter0->SetLineColor(kBlack);
  iter1->SetLineColor(kBlack);
  iter2->SetLineColor(kBlack);
  iter3->SetLineColor(kBlack);
  iter4->SetLineColor(kBlack);
  iter5->SetLineColor(kBlack);
  iter6->SetLineColor(kBlack);

  THStack hs;
  hs.Add(iter0,"HIST");
  hs.Add(iter1,"HIST");
  hs.Add(iter2,"HIST");
  hs.Add(iter3,"HIST");
  hs.Add(iter4,"HIST");
  hs.Add(iter5,"HIST");
  hs.Add(iter6,"HIST");

  TLegend* leg = 0;
  if (dovsr) leg = new TLegend(0.73,0.54,0.93,0.92);
  if (dovspt) leg = new TLegend(0.18,0.57,0.33,0.92);
  leg->SetNColumns(1);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->AddEntry(iter0,"Iter0","F");
  leg->AddEntry(iter1,"+Iter1","F");
  leg->AddEntry(iter2,"+Iter2","F");
  leg->AddEntry(iter3,"+Iter3","F");
  leg->AddEntry(iter4,"+Iter4","F");
  leg->AddEntry(iter5,"+Iter5","F");
  leg->AddEntry(iter6,"+Iter6","F");

  TPaveText* labelcms = 0;
  if (dovsr) labelcms  = new TPaveText(0.22,0.75,0.56,0.92,"NDCBR");
  if (dovspt) labelcms  = new TPaveText(0.45,0.75,0.79,0.92,"NDCBR");
  labelcms->SetTextAlign(12);
  labelcms->SetTextSize(0.035);
  labelcms->SetFillColor(kWhite);
  labelcms->AddText("CMS Preliminary Simulation");
  labelcms->AddText("#sqrt{s} = 8 TeV, t#bar{t} + <PU>=20");
  if (dovsr) labelcms->AddText("p_{T}>0.8 GeV, |#eta|<2.5");
  if (dovspt) labelcms->AddText("R_{vtx}<20 cm, |#eta|<2.5");
  labelcms->SetBorderSize(0);
  labelcms->SetTextFont(42);
  labelcms->SetLineWidth(2);

  TCanvas c1;//("c1","c1",600,600);
  iter0->SetTitle("");
  if (dovsr) {
    iter0->GetYaxis()->SetRangeUser(0.,1.);
    iter0->GetXaxis()->SetRangeUser(0.,60.);
    iter0->GetXaxis()->SetTitle("Production Radius [cm]");
  }
  if (dovspt) {
    iter0->GetYaxis()->SetRangeUser(0.,1.4);
    iter0->GetXaxis()->SetRangeUser(0.11,19.);
    iter0->GetXaxis()->SetTitle("p_{T} [GeV]");
  }
  iter0->GetYaxis()->SetTitle("Efficiency");
  iter0->GetYaxis()->SetTitleOffset(1.2);
  iter0->GetYaxis()->SetTitleSize(0.05);
  iter0->GetYaxis()->SetLabelSize(0.04);
  iter0->GetXaxis()->SetTitleOffset(1.1);
  iter0->GetXaxis()->SetTitleSize(0.05);
  iter0->GetXaxis()->SetLabelSize(0.04);
  iter0->Draw("H");
  hs.Draw("Hsame");
  iterA->Draw("E2same");
  if (dovspt) c1.SetLogx(1);
  c1.RedrawAxis();
  leg.Draw();
  labelcms->Draw();

  if (dovsr)  c1.SaveAs("effic_vs_vertpos.png");
  if (dovspt) c1.SaveAs("efficPt.png");

}
