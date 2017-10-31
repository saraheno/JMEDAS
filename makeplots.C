  
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"


void SetupCanvas(TCanvas* canv) {


  // references for T, B, L, R
  int W = 800;
  int H = 600;

  float T = 0.08*H;
  float B = 0.12*H; 
  float L = 0.12*W;
  float R = 0.04*W;
  
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetFrameFillStyle(0);
  canv->SetFrameBorderMode(0);
  canv->SetLeftMargin( L/W );
  canv->SetRightMargin( R/W );
  canv->SetTopMargin( T/H );
  canv->SetBottomMargin( B/H );
  canv->SetTickx(0);
  canv->SetTicky(0);

  float x1_l = 1.2;
  float y1_l = 0.80;
  
  float dx_l = 0.60;
  float dy_l = 0.1;
  float x0_l = x1_l-dx_l;
  float y0_l = y1_l-dy_l;
  

  
}

TH1F* getHIST(TFile* f1,char* hname) {
 std::cout<<"getting "<<hname<<std::endl;
 TH1F *A_pt = static_cast<TH1F*>(f1->Get(hname)->Clone());
 A_pt->SetDirectory(0);
 return A_pt;
}

	      //void MakePlot(TFile* f1, TCanvas* canv, char* hname, char* atitle, double aaN,int color,char* outname) {
void MakePlot(TH1F* A_pt,TCanvas* canv, char* atitle, double aaN,int color,char* outname) {

 double aaA = A_pt->Integral();
 std::cout<<" entries is "<<aaA<<std::endl;
 A_pt->Scale(1./aaN);

 A_pt->GetYaxis()->SetTitle("per event ");  
 A_pt->GetYaxis()->SetTitleSize(0.05);  
 A_pt->GetXaxis()->SetTitle(atitle);  
 A_pt->GetXaxis()->SetTitleSize(0.05);  

 A_pt->SetLineColor(color);
 A_pt->SetLineWidth(3);
 A_pt->SetStats(0);
 A_pt->Draw("");

 canv->Update();
 canv->RedrawAxis();
 canv->GetFrame()->Draw();


 canv->Print(outname,".png");


}



void makeplots() {

  TFile *f1 = new TFile("./out.root");

  char* hname_norm ="ana/h_njet";
  // get normalization histogram
  std::cout<<"getting norm hist"<<std::endl;
  TH1F *N_pt = static_cast<TH1F*>(f1->Get(hname_norm)->Clone());
  N_pt->SetDirectory(0);
  double aaN = N_pt->Integral();
  std::cout<<"normalizing to "<<aaN<<std::endl;

  // set up canvas

  TString canvName = "9_3_0_pre4";
  int W = 800;
  int H = 600;
  TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
  std::cout<<"setting up canvas"<<std::endl;
  SetupCanvas(canv);
  std::cout<<"done setting up canvas"<<std::endl;

  int color=1;


  char* hplot1 = "ana/h_njet";  
  char* outname = "plots/njet.png";
  char* atitle="number of ak4 jets pt>50 looseJet ID";
  TH1F* AA = getHIST(f1,hplot1);
  MakePlot(AA,canv,atitle,aaN,color,outname);

  hplot1 = "ana/h_ak4_pt";  
  outname = "plots/pt.png";
  atitle="jet pT ak4 pt>50 looseJet ID";
  AA=getHIST(f1,hplot1);
  MakePlot(AA,canv,atitle,aaN,color,outname);
  
  hplot1 = "ana/h_ak4_eta";  
  outname = "plots/eta.png";
  atitle="jet pT ak4 pt>50 looseJet ID";
  AA=getHIST(f1,hplot1);
  MakePlot(AA,canv,atitle,aaN,color,outname);
 
  hplot1 = "ana/h_ak4_genpt";  
  outname = "plots/ptgen.png";
  atitle="gen jet pT ak4 pt>50 looseJet ID";
  AA=getHIST(f1,hplot1);
  MakePlot(AA,canv,atitle,aaN,color,outname);
  

  // plot jet reconstruction efficiency versus pt
  hplot1 = "ana/h_ak4_genpt";  
  AA=getHIST(f1,hplot1);
  AA->Sumw2();
  hplot1 = "ana/h_ak4_genpt_match";  
  TH1F* BB=getHIST(f1,hplot1);
  BB->Sumw2();
  BB->Divide(BB,AA,1.,1.,"B");

  outname = "plots/eff.png";
  atitle="jet reconstruction efficiency versus pT ak4";
  MakePlot(BB,canv,atitle,1.,color,outname);


}


