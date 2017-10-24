#include <iostream>
#include <iomanip>
#include <locale>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "vector"
#include "vector"
using std::vector;

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

TTree          *fChain;   //!pointer to the analyzed TTree or TChain               
Int_t           fCurrent; //!current Tree number in a TChain                       

void qplot_hgc(const char* inputfilename, const char* hname, int statp) {

  // for 2D histograms
  // statp 0=false 1=true

  TFile *f = new TFile(inputfilename);

  gStyle->SetOptStat(statp);


  TString canvName = "Fig_";
  canvName += "haha";

  //  if( writeExtraText ) canvName += "-prelim";
  //if( iPos%10==0 ) canvName += "-out";                                        
  //else if( iPos%10==1 ) canvName += "-left";                                  
  //else if( iPos%10==2 )  canvName += "-center";                               
  //else if( iPos%10==3 )  canvName += "-right";                                
  int W = 800;
  int H = 600;
  TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
  // references for T, B, L, R                                                  
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




  // get histogram
    TH2F* ahist = static_cast<TH2F*>(f->Get(hname)->Clone());
    ahist->Draw("colz");

    TLine *g = new TLine(1.1,0,1.1,1); g->Draw();
    g = new TLine(-1.1,0,-1.1,1); g->Draw();

    g = new TLine(1.9,0,1.9,1); g->Draw();
    g = new TLine(-1.9,0,-1.9,1); g->Draw();

    g = new TLine(2.6,0,2.6,1); g->Draw();
    g = new TLine(-2.6,0,-2.6,1); g->Draw();

    g = new TLine(3.4,0,3.4,1); g->Draw();
    g = new TLine(-3.4,0,-3.4,1); g->Draw();




  canv->Update();
  canv->RedrawAxis();
  canv->GetFrame()->Draw();


  return;


}
