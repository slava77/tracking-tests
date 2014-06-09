// root -b -q timingTestNew.C++

#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPaveText.h"
#include <iostream>
#include <fstream>

struct TimeInfoOneStep {
  double timeClusters, timeSeeds, timeTrackCandidates, timeTracks, timeSelector, timeTotal;
};
struct TimeInfoAllSteps {
  //TimeInfoAllSteps: first argument is log file, second is mtv file,
  //third is line style, fourth is color, fifth is legend label
  TimeInfoAllSteps(TString a_logfile, TString a_mtvfile, int a_style, int a_color, TString a_legend) {
    logfile_=a_logfile;mtvfile_=a_mtvfile;style_=a_style;color_=a_color;legend_=a_legend;
  }
  TimeInfoOneStep initialStep, lowPtTripletStep, pixelPairStep, detachedTripletStep, mixedTripletStep, pixelLessStep, tobTecStep;
  TString logfile() {return logfile_;}
  TString mtvfile() {return mtvfile_;}
  TString legend() {return legend_;}
  int style() {return style_;}
  int color() {return color_;}
  TString logfile_, mtvfile_, legend_;
  int style_, color_;
};

double getTime(TString logfile, TString module) {

  gSystem->Exec("grep TimeReport "+logfile+" | grep "+module+" | tail -1 >& tmp1.log");
  gSystem->Exec("echo >> tmp1.log");

  ifstream inlimits("tmp1.log");
  string line;
  double imas = 0;
  if (inlimits.is_open()) {
    while ( inlimits.good() ) {
      getline (inlimits,line);
      TString myline(line);
      if ((*myline.Tokenize(' ')).GetEntries()<8) continue;
      TString value( ((TObjString*) (*myline.Tokenize(' '))[5])->GetString().Data() );
      imas = value.Atof();
      //cout << line << endl;
    }
    inlimits.close();
  }
  gSystem->Exec("rm tmp1.log");
  //cout << "inlimits.is_open(): " << inlimits.is_open() << endl;
  //cout<<"returning: " << imas << endl;
  return imas;

}

void getAllTimeInfo(TimeInfoAllSteps& info) {

  //bool is_pls1 = logfile.Contains("pls1");
  TString logfile = info.logfile();

  info.initialStep.timeClusters        = 0.;
  info.initialStep.timeSeeds           = getTime(logfile,"initialStepSeeds");
  info.initialStep.timeTrackCandidates = getTime(logfile,"initialStepTrackCandidates");
  info.initialStep.timeTracks          = getTime(logfile,"initialStepTracks");
  info.initialStep.timeSelector        = getTime(logfile,"initialStepSelector");
  info.initialStep.timeTotal           = info.initialStep.timeClusters+info.initialStep.timeSeeds+info.initialStep.timeTrackCandidates+info.initialStep.timeTracks+info.initialStep.timeSelector;

  info.lowPtTripletStep.timeClusters        = getTime(logfile,"lowPtTripletStepClusters");
  info.lowPtTripletStep.timeSeeds           = getTime(logfile,"lowPtTripletStepSeeds");
  info.lowPtTripletStep.timeTrackCandidates = getTime(logfile,"lowPtTripletStepTrackCandidates");
  info.lowPtTripletStep.timeTracks          = getTime(logfile,"lowPtTripletStepTracks");
  info.lowPtTripletStep.timeSelector        = getTime(logfile,"lowPtTripletStepSelector");
  info.lowPtTripletStep.timeTotal           = info.lowPtTripletStep.timeClusters+info.lowPtTripletStep.timeSeeds+info.lowPtTripletStep.timeTrackCandidates+info.lowPtTripletStep.timeTracks+info.lowPtTripletStep.timeSelector;

  info.pixelPairStep.timeClusters        = getTime(logfile,"pixelPairStepClusters");
  info.pixelPairStep.timeSeeds           = getTime(logfile,"pixelPairStepSeeds");
  info.pixelPairStep.timeTrackCandidates = getTime(logfile,"pixelPairStepTrackCandidates");
  info.pixelPairStep.timeTracks          = getTime(logfile,"pixelPairStepTracks");
  info.pixelPairStep.timeSelector        = getTime(logfile,"pixelPairStepSelector");
  info.pixelPairStep.timeTotal           = info.pixelPairStep.timeClusters+info.pixelPairStep.timeSeeds+info.pixelPairStep.timeTrackCandidates+info.pixelPairStep.timeTracks+info.pixelPairStep.timeSelector;

  info.detachedTripletStep.timeClusters        = getTime(logfile,"detachedTripletStepClusters");
  info.detachedTripletStep.timeSeeds           = getTime(logfile,"detachedTripletStepSeeds");
  info.detachedTripletStep.timeTrackCandidates = getTime(logfile,"detachedTripletStepTrackCandidates");
  info.detachedTripletStep.timeTracks          = getTime(logfile,"detachedTripletStepTracks");
  info.detachedTripletStep.timeSelector        = getTime(logfile,"detachedTripletStepSelector");
  info.detachedTripletStep.timeTotal           = info.detachedTripletStep.timeClusters+info.detachedTripletStep.timeSeeds+info.detachedTripletStep.timeTrackCandidates+info.detachedTripletStep.timeTracks+info.detachedTripletStep.timeSelector;

  info.mixedTripletStep.timeClusters        = getTime(logfile,"mixedTripletStepClusters");
  double mixedTripletStepSeedsA             = getTime(logfile,"mixedTripletStepSeedsA");
  double mixedTripletStepSeedsB             = getTime(logfile,"mixedTripletStepSeedsB");
  info.mixedTripletStep.timeSeeds           = getTime(logfile,"mixedTripletStepSeeds")+mixedTripletStepSeedsA+mixedTripletStepSeedsB;
  info.mixedTripletStep.timeTrackCandidates = getTime(logfile,"mixedTripletStepTrackCandidates");
  info.mixedTripletStep.timeTracks          = getTime(logfile,"mixedTripletStepTracks");
  info.mixedTripletStep.timeSelector        = getTime(logfile,"mixedTripletStepSelector");
  info.mixedTripletStep.timeTotal           = info.mixedTripletStep.timeClusters+info.mixedTripletStep.timeSeeds+info.mixedTripletStep.timeTrackCandidates+info.mixedTripletStep.timeTracks+info.mixedTripletStep.timeSelector;

  info.pixelLessStep.timeClusters        = getTime(logfile,"pixelLessStepClusters");
  double pixelLessStepSeedsA             = getTime(logfile,"pixelLessStepSeedsA");
  double pixelLessStepSeedsB             = getTime(logfile,"pixelLessStepSeedsB");
  info.pixelLessStep.timeSeeds           = getTime(logfile,"pixelLessStepSeeds")+pixelLessStepSeedsA+pixelLessStepSeedsB;
  info.pixelLessStep.timeTrackCandidates = getTime(logfile,"pixelLessStepTrackCandidates");
  info.pixelLessStep.timeTracks          = getTime(logfile,"pixelLessStepTracks");
  info.pixelLessStep.timeSelector        = getTime(logfile,"pixelLessStepSelector");
  info.pixelLessStep.timeTotal           = info.pixelLessStep.timeClusters+info.pixelLessStep.timeSeeds+info.pixelLessStep.timeTrackCandidates+info.pixelLessStep.timeTracks+info.pixelLessStep.timeSelector;

  info.tobTecStep.timeClusters        = getTime(logfile,"tobTecStepClusters");
  double tobTecStepSeedsA             = getTime(logfile,"tobTecStepSeedsTripl");
  double tobTecStepSeedsB             = getTime(logfile,"tobTecStepSeedsPair");
  info.tobTecStep.timeSeeds           = getTime(logfile,"tobTecStepSeeds")+tobTecStepSeedsA+tobTecStepSeedsB;
  info.tobTecStep.timeTrackCandidates = getTime(logfile,"tobTecStepTrackCandidates");
  info.tobTecStep.timeTracks          = getTime(logfile,"tobTecStepTracks");
  info.tobTecStep.timeSelector        = getTime(logfile,"tobTecStepSelector");
  info.tobTecStep.timeTotal           = info.tobTecStep.timeClusters+info.tobTecStep.timeSeeds+info.tobTecStep.timeTrackCandidates+info.tobTecStep.timeTracks+info.tobTecStep.timeSelector;

}

void timingTestPerIter(TimeInfoAllSteps& info, TH1F* htime) {

  htime->SetBinContent(1,info.initialStep.timeTotal);
  htime->SetBinContent(2,info.lowPtTripletStep.timeTotal);
  htime->SetBinContent(3,info.pixelPairStep.timeTotal);
  htime->SetBinContent(4,info.detachedTripletStep.timeTotal);
  htime->SetBinContent(5,info.mixedTripletStep.timeTotal);
  htime->SetBinContent(6,info.pixelLessStep.timeTotal);
  htime->SetBinContent(7,info.tobTecStep.timeTotal);
  htime->SetBinContent(8,htime->Integral()/7.);

  htime->GetYaxis()->SetRangeUser(0,1.1*htime->GetMaximum());
  htime->SetLineWidth(2);

  htime->GetXaxis()->SetLabelSize(0.06);
  htime->GetXaxis()->SetBinLabel(1,"iter0");
  htime->GetXaxis()->SetBinLabel(2,"iter1");
  htime->GetXaxis()->SetBinLabel(3,"iter2");
  htime->GetXaxis()->SetBinLabel(4,"iter3");
  htime->GetXaxis()->SetBinLabel(5,"iter4");
  htime->GetXaxis()->SetBinLabel(6,"iter5");
  htime->GetXaxis()->SetBinLabel(7,"iter6");
  htime->GetXaxis()->SetBinLabel(8,"average");
  htime->SetTitle("");
  htime->GetYaxis()->SetTitle("time [s]");

}

void timingTestPerStep(TimeInfoAllSteps& info, TH1F* htime) {

  double totClusters        = info.initialStep.timeClusters+info.lowPtTripletStep.timeClusters+info.pixelPairStep.timeClusters+info.detachedTripletStep.timeClusters+
                              info.mixedTripletStep.timeClusters+info.pixelLessStep.timeClusters+info.tobTecStep.timeClusters;
  double totSeeds           = info.initialStep.timeSeeds+info.lowPtTripletStep.timeSeeds+info.pixelPairStep.timeSeeds+info.detachedTripletStep.timeSeeds+
                              info.mixedTripletStep.timeSeeds+info.pixelLessStep.timeSeeds+info.tobTecStep.timeSeeds;
  double totTrackCandidates = info.initialStep.timeTrackCandidates+info.lowPtTripletStep.timeTrackCandidates+info.pixelPairStep.timeTrackCandidates+info.detachedTripletStep.timeTrackCandidates+
                              info.mixedTripletStep.timeTrackCandidates+info.pixelLessStep.timeTrackCandidates+info.tobTecStep.timeTrackCandidates;
  double totTracks          = info.initialStep.timeTracks+info.lowPtTripletStep.timeTracks+info.pixelPairStep.timeTracks+info.detachedTripletStep.timeTracks+
                              info.mixedTripletStep.timeTracks+info.pixelLessStep.timeTracks+info.tobTecStep.timeTracks;
  double totSelector        = info.initialStep.timeSelector+info.lowPtTripletStep.timeSelector+info.pixelPairStep.timeSelector+info.detachedTripletStep.timeSelector+
                              info.mixedTripletStep.timeSelector+info.pixelLessStep.timeSelector+info.tobTecStep.timeSelector;

  htime->SetBinContent(1,totClusters);
  htime->SetBinContent(2,totSeeds);
  htime->SetBinContent(3,totTrackCandidates);
  htime->SetBinContent(4,totTracks);
  htime->SetBinContent(5,totSelector);

  htime->GetYaxis()->SetRangeUser(0,1.1*htime->GetMaximum());
  htime->SetLineWidth(2);

  htime->GetXaxis()->SetLabelSize(0.06);
  htime->GetXaxis()->SetBinLabel(1,"masking");
  htime->GetXaxis()->SetBinLabel(2,"seeding");
  htime->GetXaxis()->SetBinLabel(3,"building");
  htime->GetXaxis()->SetBinLabel(4,"fitting");
  htime->GetXaxis()->SetBinLabel(5,"selection");
  htime->SetTitle("");
  htime->GetYaxis()->SetTitle("time [s]");

}

void makeTimePerTrackPlot(TH1F* htime_track, TH1F* htime_iter, TString mtvfilename) {

  TFile* f = TFile::Open(mtvfilename);
  TH1F* h_algo = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/h_algo");
  TH1F* h_entries = (TH1F*) f->Get("DQMData/Tracking/Track/cutsReco_quickAssociatorByHits/fakes");
  h_algo->Scale(1./float(h_entries->GetEntries()));
  for (int bin=1;bin<8;++bin) {
    htime_track->SetBinContent(bin,htime_iter->GetBinContent(bin)/h_algo->GetBinContent(bin));
  }
  htime_track->SetBinContent(8,htime_iter->Integral(1,7)/h_algo->Integral());
  f->Close();

}

void plotOneVersion(TimeInfoAllSteps TIAS,TString file_postfix) {

  gStyle->SetOptStat(0);

  TH1F *htime_iter = new TH1F("time_iter"+file_postfix,"time_iter",8,0,8);
  timingTestPerIter(TIAS,htime_iter);
  htime_iter->SetLineStyle(TIAS.style());
  htime_iter->SetLineColor(TIAS.color());
  TLegend* leg = new TLegend(0.1,0.91,0.9,0.99);
  leg->SetNColumns(1);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->AddEntry(htime_iter,TIAS.legend(),"L");
  TCanvas c0;
  c0.SetTicks(1,1);
  htime_iter->GetYaxis()->SetRangeUser(0,1.1*htime_iter->GetBinContent(htime_iter->GetMaximumBin()));
  htime_iter->Draw("H");
  leg->Draw();
  c0.SetGridy();
  c0.SaveAs("timingNew_iter_"+file_postfix+".png");

  TH1F *htime_step = new TH1F("time_step"+file_postfix,"time_step",5,0,5);
  timingTestPerStep(TIAS,htime_step);
  htime_step->SetLineStyle(TIAS.style());
  htime_step->SetLineColor(TIAS.color());
  htime_step->GetYaxis()->SetTitle("time [s]");
  TCanvas c1;
  c1.SetTicks(1,1);
  //htime_step->Scale(1./htime_step->Integral());
  htime_step->GetYaxis()->SetRangeUser(0,1.1*htime_step->GetBinContent(htime_step->GetMaximumBin()));
  htime_step->Draw("H");
  leg->Draw();
  c1.SetGridy();
  c1.SaveAs("timingNew_step_"+file_postfix+".png");

  TH1F *htime_track = (TH1F*) htime_iter->Clone("time_track"+file_postfix);
  htime_track->Reset();
  makeTimePerTrackPlot(htime_track,htime_iter,TIAS.mtvfile());
  htime_track->GetYaxis()->SetRangeUser(0,1.1*htime_track->GetBinContent(htime_track->GetMaximumBin()));
  htime_track->GetYaxis()->SetTitle("time/HP track [s]");
  htime_track->GetYaxis()->SetTitleOffset(1.25);
  TCanvas c2;
  c2.SetTicks(1,1);
  htime_track->Draw("H");
  leg->Draw();
  c2.SetGridy();
  c2.SaveAs("timingNew_track_"+file_postfix+".png");

//   delete htime_iter;
//   delete htime_step;
//   delete htime_track;
//   delete leg;

}

void compareTwoVersions(TimeInfoAllSteps TIAS_old, TimeInfoAllSteps TIAS_new,TString file_postfix) {

  gStyle->SetOptStat(0);

  TH1F *htime_iter_old = new TH1F("time_iter_old"+file_postfix,"time_iter_old",8,0,8);
  timingTestPerIter(TIAS_old,htime_iter_old);
  htime_iter_old->SetLineStyle(TIAS_old.style());
  htime_iter_old->SetLineColor(TIAS_old.color());
  TH1F *htime_iter_new = new TH1F("time_iter_new"+file_postfix,"time_iter_new",8,0,8);
  timingTestPerIter(TIAS_new,htime_iter_new);
  htime_iter_new->SetLineStyle(TIAS_new.style());
  htime_iter_new->SetLineColor(TIAS_new.color());
  htime_iter_old->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_iter_old->GetBinContent(htime_iter_old->GetMaximumBin()),htime_iter_new->GetBinContent(htime_iter_new->GetMaximumBin())));
  TLegend* leg = new TLegend(0.1,0.91,0.9,0.99);
  leg->SetNColumns(2);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->AddEntry(htime_iter_old,TIAS_old.legend(),"L");
  leg->AddEntry(htime_iter_new,TIAS_new.legend(),"L");
  TCanvas c0;
  c0.SetTicks(1,1);
  htime_iter_old->Draw("H");
  htime_iter_new->Draw("H,same");
  leg->Draw();
  c0.SetGridy();
  c0.SaveAs("timingNew_iter_"+file_postfix+".png");

  TH1F *htime_step_old = new TH1F("time_step_old"+file_postfix,"time_step_old",5,0,5);
  timingTestPerStep(TIAS_old,htime_step_old);
  htime_step_old->SetLineStyle(TIAS_old.style());
  htime_step_old->SetLineColor(TIAS_old.color());
  htime_step_old->GetYaxis()->SetTitle("time [s]");
  TH1F *htime_step_new = new TH1F("time_step_new"+file_postfix,"time_step_new",5,0,5);
  timingTestPerStep(TIAS_new,htime_step_new);
  htime_step_new->SetLineStyle(TIAS_new.style());
  htime_step_new->SetLineColor(TIAS_new.color());
  TCanvas c1;
  c1.SetTicks(1,1);
  //htime_step_old->Scale(1./htime_step_old->Integral());
  //htime_step_new->Scale(1./htime_step_new->Integral());
  htime_step_old->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_step_old->GetBinContent(htime_step_old->GetMaximumBin()),htime_step_new->GetBinContent(htime_step_new->GetMaximumBin())));
  htime_step_old->Draw("H");
  htime_step_new->Draw("H,same");
  leg->Draw();
  c1.SetGridy();
  c1.SaveAs("timingNew_step_"+file_postfix+".png");

  TH1F *htime_track_old = (TH1F*) htime_iter_old->Clone("time_track_old"+file_postfix);
  htime_track_old->Reset();
  makeTimePerTrackPlot(htime_track_old,htime_iter_old,TIAS_old.mtvfile());
  TH1F *htime_track_new = (TH1F*) htime_iter_new->Clone("time_track_new"+file_postfix);
  htime_track_new->Reset();
  makeTimePerTrackPlot(htime_track_new,htime_iter_new,TIAS_new.mtvfile());
  htime_track_old->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_track_old->GetBinContent(htime_track_old->GetMaximumBin()),htime_track_new->GetBinContent(htime_track_new->GetMaximumBin())));
  htime_track_old->GetYaxis()->SetTitle("time/HP track [s]");
  htime_track_old->GetYaxis()->SetTitleOffset(1.25);
  TCanvas c2;
  c2.SetTicks(1,1);
  htime_track_old->Draw("H");
  htime_track_new->Draw("H,same");
  leg->Draw();
  c2.SetGridy();
  c2.SaveAs("timingNew_track_"+file_postfix+".png");

}

void compareThreeVersions(TimeInfoAllSteps TIAS_1,TimeInfoAllSteps TIAS_2,TimeInfoAllSteps TIAS_3,TString file_postfix) {

  gStyle->SetOptStat(0);

  TPaveText* labelcms  = new TPaveText(0.20,0.79,0.54,0.92,"NDCBR");
  labelcms->SetTextAlign(12);
  labelcms->SetTextSize(0.04);
  labelcms->SetFillColor(kWhite);  
  labelcms->AddText("CMS Preliminary Simulation");
  //labelcms->AddText("CMS Preliminary");
  //labelcms->AddText("Simulation");
  labelcms->AddText("#sqrt{s} = 8 TeV, t#bar{t}+PU");
  labelcms->SetBorderSize(0);
  labelcms->SetTextFont(42);
  labelcms->SetLineWidth(2);

  TH1F *htime_iter_1 = new TH1F("time_iter_1"+file_postfix,"time_iter_1",8,0,8);
  timingTestPerIter(TIAS_1,htime_iter_1);
  htime_iter_1->SetLineStyle(TIAS_1.style());
  htime_iter_1->SetLineColor(TIAS_1.color());
  TH1F *htime_iter_2 = new TH1F("time_iter_2"+file_postfix,"time_iter_2",8,0,8);
  timingTestPerIter(TIAS_2,htime_iter_2);
  htime_iter_2->SetLineStyle(TIAS_2.style());
  htime_iter_2->SetLineColor(TIAS_2.color());
  TH1F *htime_iter_3 = new TH1F("time_iter_3"+file_postfix,"time_iter_3",8,0,8);
  timingTestPerIter(TIAS_3,htime_iter_3);
  htime_iter_3->SetLineStyle(TIAS_3.style());
  htime_iter_3->SetLineColor(TIAS_3.color());
  float iter_Iter0PU20 = htime_iter_1->GetBinContent(1);
  htime_iter_1->Scale(1./iter_Iter0PU20);
  htime_iter_2->Scale(1./iter_Iter0PU20);
  htime_iter_3->Scale(1./iter_Iter0PU20);
  htime_iter_1->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_iter_1->GetBinContent(htime_iter_1->GetMaximumBin()),TMath::Max(htime_iter_2->GetBinContent(htime_iter_2->GetMaximumBin()),htime_iter_3->GetBinContent(htime_iter_3->GetMaximumBin()))));
  TLegend* leg = new TLegend(0.2,0.59,0.45,0.79);
  leg->SetNColumns(1);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->AddEntry(htime_iter_1,TIAS_1.legend(),"L");
  leg->AddEntry(htime_iter_2,TIAS_2.legend(),"L");
  leg->AddEntry(htime_iter_3,TIAS_3.legend(),"L");
  htime_iter_1->GetYaxis()->SetTitleOffset(1.2);
  htime_iter_1->GetXaxis()->SetRangeUser(0,7);
  htime_iter_1->GetYaxis()->SetTitle("time [1/iter0@<PU>=20]");
  htime_iter_1->GetYaxis()->SetTitleSize(0.04);
  htime_iter_1->GetXaxis()->SetTitleSize(0.04);
  htime_iter_1->GetYaxis()->SetLabelSize(0.04);
  TCanvas c0;
  c0.SetTicks(1,1);
  htime_iter_1->Draw("H");
  htime_iter_2->Draw("H,same");
  htime_iter_3->Draw("H,same");
  leg->Draw();labelcms->Draw();
  c0.SetGridy();
  c0.SaveAs("timingNew_iter_"+file_postfix+".png");

  TH1F *htime_step_1 = new TH1F("time_step_1"+file_postfix,"time_step_1",5,0,5);
  timingTestPerStep(TIAS_1,htime_step_1);
  htime_step_1->SetLineStyle(TIAS_1.style());
  htime_step_1->SetLineColor(TIAS_1.color());
  htime_step_1->GetYaxis()->SetTitle("time [1/building@<PU>=20]");
  TH1F *htime_step_2 = new TH1F("time_step_2"+file_postfix,"time_step_2",5,0,5);
  timingTestPerStep(TIAS_2,htime_step_2);
  htime_step_2->SetLineStyle(TIAS_2.style());
  htime_step_2->SetLineColor(TIAS_2.color());
  TH1F *htime_step_3 = new TH1F("time_step_3"+file_postfix,"time_step_3",5,0,5);
  timingTestPerStep(TIAS_3,htime_step_3);
  htime_step_3->SetLineStyle(TIAS_3.style());
  htime_step_3->SetLineColor(TIAS_3.color());
  float step_BuildPU20 = htime_step_1->GetBinContent(3);
  htime_step_1->Scale(1./step_BuildPU20);
  htime_step_2->Scale(1./step_BuildPU20);
  htime_step_3->Scale(1./step_BuildPU20);
  htime_step_1->GetYaxis()->SetTitleOffset(1.2);
  htime_step_1->GetXaxis()->SetRangeUser(0,7);
  htime_step_1->GetYaxis()->SetTitleSize(0.04);
  htime_step_1->GetXaxis()->SetTitleSize(0.04);
  htime_step_1->GetYaxis()->SetLabelSize(0.04);
  TCanvas c1;
  c1.SetTicks(1,1);
  //htime_step_1->Scale(1./htime_step_1->Integral());
  //htime_step_2->Scale(1./htime_step_2->Integral());
  //htime_step_3->Scale(1./htime_step_3->Integral());
  htime_step_1->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_step_1->GetBinContent(htime_step_1->GetMaximumBin()),TMath::Max(htime_step_2->GetBinContent(htime_step_2->GetMaximumBin()),htime_step_3->GetBinContent(htime_step_3->GetMaximumBin()))));
  htime_step_1->GetXaxis()->SetRangeUser(1,5);
  htime_step_1->Draw("H");
  htime_step_2->Draw("H,same");
  htime_step_3->Draw("H,same");
  leg->SetX1NDC(0.60);
  leg->SetX2NDC(0.85);
  labelcms->SetX1NDC(0.60);
  labelcms->SetX2NDC(0.94);
  leg->Draw();labelcms->Draw();
  c1.SetGridy();
  c1.SaveAs("timingNew_step_"+file_postfix+".png");

  TH1F *htime_track_1 = (TH1F*) htime_iter_1->Clone("time_track_1"+file_postfix);
  htime_track_1->Reset();
  makeTimePerTrackPlot(htime_track_1,htime_iter_1,TIAS_1.mtvfile());
  TH1F *htime_track_2 = (TH1F*) htime_iter_2->Clone("time_track_2"+file_postfix);
  htime_track_2->Reset();
  makeTimePerTrackPlot(htime_track_2,htime_iter_2,TIAS_2.mtvfile());
  TH1F *htime_track_3 = (TH1F*) htime_iter_3->Clone("time_track_3"+file_postfix);
  htime_track_3->Reset();
  makeTimePerTrackPlot(htime_track_3,htime_iter_3,TIAS_3.mtvfile());
  float track_Iter0PU20 = htime_track_1->GetBinContent(1);
  htime_track_1->Scale(1./track_Iter0PU20);
  htime_track_2->Scale(1./track_Iter0PU20);
  htime_track_3->Scale(1./track_Iter0PU20);
  htime_track_1->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_track_1->GetBinContent(htime_track_1->GetMaximumBin()),TMath::Max(htime_track_2->GetBinContent(htime_track_2->GetMaximumBin()),htime_track_3->GetBinContent(htime_track_3->GetMaximumBin()))));
  TCanvas c2;
  c2.SetTicks(1,1);
  htime_track_1->GetYaxis()->SetTitleOffset(1.2);
  htime_track_1->GetXaxis()->SetRangeUser(0,7);
  htime_track_1->GetYaxis()->SetTitle("time/track [1/iter0@<PU>=20]");
  htime_track_1->GetYaxis()->SetTitleSize(0.04);
  htime_track_1->GetXaxis()->SetTitleSize(0.04);
  htime_track_1->GetYaxis()->SetLabelSize(0.04);
  htime_track_1->Draw("H");
  htime_track_2->Draw("H,same");
  htime_track_3->Draw("H,same");
  leg->SetX1NDC(0.2);
  leg->SetX2NDC(0.45);
  labelcms->SetX1NDC(0.2);
  labelcms->SetX2NDC(0.54);
  leg->Draw();labelcms->Draw();
  c2.SetGridy();
  c2.SaveAs("timingNew_track_"+file_postfix+".png");

}

void compareFourVersions(TimeInfoAllSteps TIAS_1,TimeInfoAllSteps TIAS_2,TimeInfoAllSteps TIAS_3,TimeInfoAllSteps TIAS_4,TString file_postfix) {

  gStyle->SetOptStat(0);

  TH1F *htime_iter_1 = new TH1F("time_iter_1"+file_postfix,"time_iter_1",8,0,8);
  timingTestPerIter(TIAS_1,htime_iter_1);
  htime_iter_1->SetLineStyle(TIAS_1.style());
  htime_iter_1->SetLineColor(TIAS_1.color());
  TH1F *htime_iter_2 = new TH1F("time_iter_2"+file_postfix,"time_iter_2",8,0,8);
  timingTestPerIter(TIAS_2,htime_iter_2);
  htime_iter_2->SetLineStyle(TIAS_2.style());
  htime_iter_2->SetLineColor(TIAS_2.color());
  TH1F *htime_iter_3 = new TH1F("time_iter_3"+file_postfix,"time_iter_3",8,0,8);
  timingTestPerIter(TIAS_3,htime_iter_3);
  htime_iter_3->SetLineStyle(TIAS_3.style());
  htime_iter_3->SetLineColor(TIAS_3.color());
  TH1F *htime_iter_4 = new TH1F("time_iter_4"+file_postfix,"time_iter_4",8,0,8);
  timingTestPerIter(TIAS_4,htime_iter_4);
  htime_iter_4->SetLineStyle(TIAS_4.style());
  htime_iter_4->SetLineColor(TIAS_4.color());
  htime_iter_1->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_iter_1->GetBinContent(htime_iter_1->GetMaximumBin()),TMath::Max(htime_iter_2->GetBinContent(htime_iter_2->GetMaximumBin()),TMath::Max(htime_iter_3->GetBinContent(htime_iter_3->GetMaximumBin()),htime_iter_4->GetBinContent(htime_iter_4->GetMaximumBin())))));
  TLegend* leg = new TLegend(0.1,0.91,0.9,0.99);
  leg->SetNColumns(4);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->AddEntry(htime_iter_1,TIAS_1.legend(),"L");
  leg->AddEntry(htime_iter_2,TIAS_2.legend(),"L");
  leg->AddEntry(htime_iter_3,TIAS_3.legend(),"L");
  leg->AddEntry(htime_iter_4,TIAS_4.legend(),"L");
  TCanvas c0;
  c0.SetTicks(1,1);
  htime_iter_1->Draw("H");
  htime_iter_2->Draw("H,same");
  htime_iter_3->Draw("H,same");
  htime_iter_4->Draw("H,same");
  leg->Draw();
  c0.SetGridy();
  c0.SaveAs("timingNew_iter_"+file_postfix+".png");

  TH1F *htime_step_1 = new TH1F("time_step_1"+file_postfix,"time_step_1",5,0,5);
  timingTestPerStep(TIAS_1,htime_step_1);
  htime_step_1->SetLineStyle(TIAS_1.style());
  htime_step_1->SetLineColor(TIAS_1.color());
  htime_step_1->GetYaxis()->SetTitle("time [s]");
  TH1F *htime_step_2 = new TH1F("time_step_2"+file_postfix,"time_step_2",5,0,5);
  timingTestPerStep(TIAS_2,htime_step_2);
  htime_step_2->SetLineStyle(TIAS_2.style());
  htime_step_2->SetLineColor(TIAS_2.color());
  TH1F *htime_step_3 = new TH1F("time_step_3"+file_postfix,"time_step_3",5,0,5);
  timingTestPerStep(TIAS_3,htime_step_3);
  htime_step_3->SetLineStyle(TIAS_3.style());
  htime_step_3->SetLineColor(TIAS_3.color());
  TH1F *htime_step_4 = new TH1F("time_step_4"+file_postfix,"time_step_4",5,0,5);
  timingTestPerStep(TIAS_4,htime_step_4);
  htime_step_4->SetLineStyle(TIAS_4.style());
  htime_step_4->SetLineColor(TIAS_4.color());
  TCanvas c1;
  c1.SetTicks(1,1);
  //htime_step_1->Scale(1./htime_step_1->Integral());
  //htime_step_2->Scale(1./htime_step_2->Integral());
  //htime_step_3->Scale(1./htime_step_3->Integral());
  //htime_step_4->Scale(1./htime_step_4->Integral());
  htime_step_1->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_step_1->GetBinContent(htime_step_1->GetMaximumBin()),TMath::Max(htime_step_2->GetBinContent(htime_step_2->GetMaximumBin()),TMath::Max(htime_step_3->GetBinContent(htime_step_3->GetMaximumBin()),htime_step_4->GetBinContent(htime_step_4->GetMaximumBin())))));
  htime_step_1->Draw("H");
  htime_step_2->Draw("H,same");
  htime_step_3->Draw("H,same");
  htime_step_4->Draw("H,same");
  leg->Draw();
  c1.SetGridy();
  c1.SaveAs("timingNew_step_"+file_postfix+".png");

  TH1F *htime_track_1 = (TH1F*) htime_iter_1->Clone("time_track_1"+file_postfix);
  htime_track_1->Reset();
  makeTimePerTrackPlot(htime_track_1,htime_iter_1,TIAS_1.mtvfile());
  TH1F *htime_track_2 = (TH1F*) htime_iter_2->Clone("time_track_2"+file_postfix);
  htime_track_2->Reset();
  makeTimePerTrackPlot(htime_track_2,htime_iter_2,TIAS_2.mtvfile());
  TH1F *htime_track_3 = (TH1F*) htime_iter_3->Clone("time_track_3"+file_postfix);
  htime_track_3->Reset();
  makeTimePerTrackPlot(htime_track_3,htime_iter_3,TIAS_3.mtvfile());
  TH1F *htime_track_4 = (TH1F*) htime_iter_4->Clone("time_track_4"+file_postfix);
  htime_track_4->Reset();
  makeTimePerTrackPlot(htime_track_4,htime_iter_4,TIAS_4.mtvfile());
  htime_track_1->GetYaxis()->SetRangeUser(0,1.1*TMath::Max(htime_track_1->GetBinContent(htime_track_1->GetMaximumBin()),TMath::Max(htime_track_2->GetBinContent(htime_track_2->GetMaximumBin()),TMath::Max(htime_track_3->GetBinContent(htime_track_3->GetMaximumBin()),htime_track_4->GetBinContent(htime_track_4->GetMaximumBin())))));
  htime_track_1->GetYaxis()->SetTitle("time/HP track [s]");
  htime_track_1->GetYaxis()->SetTitleOffset(1.25);
  TCanvas c2;
  c2.SetTicks(1,1);
  htime_track_1->Draw("H");
  htime_track_2->Draw("H,same");
  htime_track_3->Draw("H,same");
  htime_track_4->Draw("H,same");
  leg->Draw();
  c2.SetGridy();
  c2.SaveAs("timingNew_track_"+file_postfix+".png");

}

void makeIterPlotVsPU(TimeInfoAllSteps TIAS_pu20,TimeInfoAllSteps TIAS_pu40,TimeInfoAllSteps TIAS_pu60) {
  gStyle->SetOptStat(0);
  TCanvas c1;
  TH1F* hIter0 = new TH1F("hIter0","hIter0",3,10,70);
  hIter0->SetBinContent(1,TIAS_pu20.initialStep.timeTotal/TIAS_pu20.initialStep.timeTotal);
  hIter0->SetBinContent(2,TIAS_pu40.initialStep.timeTotal/TIAS_pu20.initialStep.timeTotal);
  hIter0->SetBinContent(3,TIAS_pu60.initialStep.timeTotal/TIAS_pu20.initialStep.timeTotal);
  hIter0->SetLineWidth(2);
  hIter0->SetLineColor(kBlack);
  TH1F* hIter5 = new TH1F("hIter5","hIter5",3,10,70);
  hIter5->SetBinContent(1,TIAS_pu20.pixelLessStep.timeTotal/TIAS_pu20.pixelLessStep.timeTotal);
  hIter5->SetBinContent(2,TIAS_pu40.pixelLessStep.timeTotal/TIAS_pu20.pixelLessStep.timeTotal);
  hIter5->SetBinContent(3,TIAS_pu60.pixelLessStep.timeTotal/TIAS_pu20.pixelLessStep.timeTotal);
  hIter5->SetLineWidth(2);
  hIter5->SetLineColor(kRed);
  hIter0->SetTitle("");
  hIter0->GetYaxis()->SetTitle("time/time@PU=20");
  hIter0->GetXaxis()->SetTitle("<PU>");
  hIter0->GetXaxis()->SetNdivisions(5);
  hIter0->GetYaxis()->SetRangeUser(0,20.);
  hIter0->Draw("H");
  hIter5->Draw("Hsame");
  TLegend* leg = new TLegend(0.1,0.91,0.9,0.99);
  leg->SetNColumns(2);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->AddEntry(hIter0,"Iter0","L");
  leg->AddEntry(hIter5,"Iter5","L");
  leg->Draw();
  c1.SetTicks(0,1);
  c1.SaveAs("iter05_vs_PU.png");
}

void timingTestNew() {

  //create TimeInfoAllSteps: first argument is log file, second is mtv file,
  //third is line style, fourth is color, fifth is legend label

  TimeInfoAllSteps TIAS_pu20("step3_20PU50ns_RAW2DIGI_L1Reco_RECO.log","mtv_20PU50ns.root",3,kBlack,"<PU>=20");
  getAllTimeInfo(TIAS_pu20);
  //plotOneVersion(TIAS_pu20,"pu20");

  TimeInfoAllSteps TIAS_pu40("step3_40PU50ns_RAW2DIGI_L1Reco_RECO.log","mtv_40PU50ns.root",1,kBlack,"<PU>=40");
  getAllTimeInfo(TIAS_pu40);
  //plotOneVersion(TIAS_pu40,"pu40");

  TimeInfoAllSteps TIAS_pu60("step3_60PU50ns_RAW2DIGI_L1Reco_RECO.log","mtv_60PU50ns.root",7,kBlack,"<PU>=60");
  getAllTimeInfo(TIAS_pu60);
  //plotOneVersion(TIAS_pu60,"pu60");

  compareThreeVersions(TIAS_pu20,TIAS_pu40,TIAS_pu60,"comp_pu_20_40_60");

//   compareTwoVersions(TIAS_default,TIAS_triplets,"comp2_def_tripl");
//   compareThreeVersions(TIAS_default,TIAS_triplets,TIAS_triplets_mtec,"comp3_def_tripl_mtec");
//   compareTwoVersions(TIAS_triplets_mtec,TIAS_triplets_mtec_ring3,"comp2_mtec_mtecr3");

//   compareThreeVersions(TIAS_default,TIAS_triplets_mtec_ring3,TIAS_triplets_mtec_ring3_tobtec,"comp3_def_mtecr3_mtecr3tobtec");
}
