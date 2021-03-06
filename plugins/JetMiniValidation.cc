// -*- C++ -*-
//
// Package:    Analysis/JetMiniValidation
// Class:      JetMiniValidation
// 
/**\class JetMiniValidation JetMiniValidation.cc Analysis/JetMiniValidation/plugins/JetMiniValidation.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  James Dolen
//         Created:  Sat, 30 Apr 2016 17:40:42 GMT
//
//

// system include files
#include <memory>
#include <stdlib.h> // For rand()     


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/transform.h"

// DataFormats
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "RecoLocalCalo/HGCalRecAlgos/interface/HGCalImagingAlgo.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "Geometry/CaloTopology/interface/HGCalTopology.h"
#include "DataFormats/ForwardDetId/interface/HGCEEDetId.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"


#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"

// PCaloHits objects
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

// HGCAL & HCAL Geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "SimDataFormats/CaloTest/interface/HGCalTestNumbering.h"
#include "DataFormats/HcalDetId/interface/HcalTestNumbering.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/HGCalCommonData/interface/HGCalDDDConstants.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"

#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"


// TFile
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// Gen particle
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// JEC
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// root
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TLorentzVector.h"
//
// class declaration
//


const int npart=27;
const char *partNames[npart] = {
  "pi0",
  "rho0",
  "pi+-",
  "rho+-",
  "eta",
  "omega",
  "K0",
  "K*",
  "K+-",
  "K*+-",
  "etaprime",
  "phi",
  "Delta-",
  "n",
  "Delta0",
  "p",
  "Delta+",
  "Delta++",
  "Sigma-",
  "Sigma*-",
  "Lambda",
  "Sigma0",
  "Sigma*0",
  "Sigma+",
  "Omega*0",
  "gamma",
  "unknown"
};






class JetMiniValidation : public edm::one::EDAnalyzer<edm::one::WatchRuns,edm::one::SharedResources>  {
public:
  explicit JetMiniValidation(const edm::ParameterSet&);
  ~JetMiniValidation();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);




private:
  virtual void beginJob() override;
  virtual void beginRun(const edm::Run& run,const edm::EventSetup& c) override;
  virtual void endRun(edm::Run const&, edm::EventSetup const&) override {}
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  // ----------member data ---------------------------
  edm::EDGetTokenT<reco::PFJetCollection> ak4jetToken_;
  edm::EDGetTokenT<reco::GenJetCollection> ak4genjetToken_;

  edm::EDGetTokenT<HGCRecHitCollection> hits_ee_token;
  edm::EDGetTokenT<HGCRecHitCollection> hits_fh_token;
  edm::EDGetTokenT<HGCRecHitCollection> hits_bh_token;

  hgcal::RecHitTools rhtools_;

  std::vector<edm::InputTag> m_PCaloHitsTags;
  std::vector<edm::EDGetTokenT<edm::PCaloHitContainer>> m_PCaloHitsTokens;

  std::vector<std::string> m_geometrySource;

  const std::string     m_prefix;
  const std::string     m_suffix;

  //HGC Geometry                                                                                                     
  std::vector<const HGCalDDDConstants*> hgcCons_;
  std::vector<const HGCalGeometry*>     hgcGeometry_;
  const HcalDDDSimConstants*            hcCons_;
  const HcalDDDRecConstants*            hcConr_;
  const CaloSubdetectorGeometry*        hcGeometry_;

  std::map<uint32_t, HepGeom::Transform3D> transMap_;



  std::unordered_map<std::string, TH1*> histoMap1D_;
  std::unordered_map<int,std::string> pdgName;
  std::unordered_map<int,int> pdgNum;




  TH1D * h_ak4_genpt             ;  
  TH1D * h_ak4_genpt_match             ;  
  TH1D * h_njet ;
  TH1D * h_ak4_pt             ;  
  TH1D * h_ak4_eta            ;  
  TH1D * h_ak4_phi            ;  
  TH1D * h_ak4_ndau           ;  
  TH1D * h_nhf ;
  TH1D * h_dr_jet             ;
  TH1D * h_dr_jet_all             ;
  TH2D * h_dr_jet_dRpt             ;
  TH1D * h_ak4_flavParton     ;
  TH1D * h_ak4_flavHadron     ;
  TH1D * h_ak4_dRminGen       ;
  TH1D * h_ak4_ptifmatchGen          ;
  TH1D * h_ak4_ptResponse     ;
  TH2D * h_res_v_pt          ;
  TH2D * h_pt1_pt2;
  TH1D * h_pt3;
  TH1D * h_pt4;
  TH1D * h_pt5;

  TH1D * h_res_pt2550;
  TH1D * h_res_pt5075;
  TH1D * h_res_pt75100;
  TH1D * h_res_pt100150;
  TH1D * h_res_pt150200;
  TH1D * h_res_pt200250;
  TH1D * h_res_pt250300;
  TH1D * h_res_pt300400;

  TH1D * h_pt2550;
  TH1D * h_pt5075;
  TH1D * h_pt75100;
  TH1D * h_pt100150;
  TH1D * h_pt150200;
  TH1D * h_pt200250;
  TH1D * h_pt250300;
  TH1D * h_pt300400;

  TH1D * h_ak4_b_pt             ;  
  TH1D * h_ak4_bhgc_pt             ;  
  TH1D * h_ak4_hgc_pt             ;  
  TH1D * h_ak4_hgchf_pt             ;  
  TH1D * h_ak4_hf_pt             ;  


  TH1D * h_ak4_b_nhf             ;  
  TH1D * h_ak4_bhgc_nhf             ;  
  TH1D * h_ak4_hgc_nhf             ;  
  TH1D * h_ak4_hgchf_nhf             ;  
  TH1D * h_ak4_hf_nhf             ;  

  TH1D * h_ak4_b_nemf             ;  
  TH1D * h_ak4_bhgc_nemf             ;  
  TH1D * h_ak4_hgc_nemf             ;  
  TH1D * h_ak4_hgchf_nemf             ;  
  TH1D * h_ak4_hf_nemf             ;  

  TH1D * h_ak4_b_chf             ;  
  TH1D * h_ak4_bhgc_chf             ;  
  TH1D * h_ak4_hgc_chf             ;  
  TH1D * h_ak4_hgchf_chf             ;  
  TH1D * h_ak4_hf_chf             ;  

  TH2D * h_ak4_hgc_chfeta;

  TH2D * h_ak4_nhfeta;
  TH2D * h_ak4_nemfeta;
  TH2D * h_ak4_chfeta;
  TH2D * h_ak4_cemfeta;



  TH1D * h_ak4_b_cemf             ;  
  TH1D * h_ak4_bhgc_cemf             ;  
  TH1D * h_ak4_hgc_cemf             ;  
  TH1D * h_ak4_hgchf_cemf             ;  
  TH1D * h_ak4_hf_cemf             ;  


  TH1D * h_ak4_b_res             ;  
  TH1D * h_ak4_bhgc_res             ;  
  TH1D * h_ak4_hgc_res             ;  
  TH1D * h_ak4_hgchf_res             ;  
  TH1D * h_ak4_hf_res             ;  

  TH2D * h_ineff_etaphi;
  TH2D * h_ineff_etapt;

  TTree *  ak4JetTree;
  Float_t  ak4_pt;
  Float_t  ak4_eta;
  Float_t  ak4_flavHadron;
  Float_t  ak4_flavParton;
  Float_t  ak4_ptGen;
  Float_t  ak4_ptResponse;
  Float_t  ak4_dRminGen;
  Float_t  ak4_area;





};

//
// constructors and destructor
//

JetMiniValidation::JetMiniValidation(const edm::ParameterSet& iConfig):
  ak4jetToken_(consumes<reco::PFJetCollection>(edm::InputTag("ak4PFJets"))),
  ak4genjetToken_(consumes<reco::GenJetCollection>(edm::InputTag("ak4GenJets"))),
  hits_ee_token(consumes<HGCRecHitCollection>(edm::InputTag("HGCalRecHit:HGCEERecHits"))),
  hits_fh_token(consumes<HGCRecHitCollection>(edm::InputTag("HGCalRecHit:HGCHEFRecHits"))),
  hits_bh_token(consumes<HGCRecHitCollection>(edm::InputTag("HGCalRecHit:HGCHEBRecHits"))),
  m_PCaloHitsTags (iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("source")),
  m_PCaloHitsTokens(edm::vector_transform(m_PCaloHitsTags, [this](edm::InputTag const & tag){
	return consumes<edm::PCaloHitContainer>(tag);})),
  m_geometrySource (iConfig.getUntrackedParameter<std::vector<std::string> >("geometrySource")),
  m_prefix         (iConfig.getUntrackedParameter<std::string>  ("Prefix")),
  m_suffix         (iConfig.getUntrackedParameter<std::string>  ("Suffix"))
{

  usesResource("TFileService");
  edm::Service<TFileService> fs;



  pdgName.emplace(111,"pi0");
  pdgName.emplace(113,"rho0");
  pdgName.emplace(211,"pi+-");
  pdgName.emplace(213,"rho+-");
  pdgName.emplace(221,"eta");
  pdgName.emplace(223,"omega");
  pdgName.emplace(311,"K0");
  pdgName.emplace(313,"K*");
  pdgName.emplace(321,"K+-");
  pdgName.emplace(323,"K*+-");
  pdgName.emplace(331,"etaprime");
  pdgName.emplace(333,"phi");
  pdgName.emplace(1114,"Delta-");
  pdgName.emplace(2112,"n");
  pdgName.emplace(2114,"Delta0");
  pdgName.emplace(2212,"p");
  pdgName.emplace(2214,"Delta+");
  pdgName.emplace(2224,"Delta++");
  pdgName.emplace(3112,"Sigma-");
  pdgName.emplace(3114,"Sigma*-");
  pdgName.emplace(3122,"Lambda");
  pdgName.emplace(3212,"Sigma0");
  pdgName.emplace(3214,"Sigma*0");
  pdgName.emplace(3222,"Sigma+");
  pdgName.emplace(3324,"Omega*0");
  pdgName.emplace(22,"gamma");

  pdgNum.emplace(111,0);
  pdgNum.emplace(113,1);
  pdgNum.emplace(211,2);
  pdgNum.emplace(213,3);
  pdgNum.emplace(221,4);
  pdgNum.emplace(223,5);
  pdgNum.emplace(311,6);
  pdgNum.emplace(313,7);
  pdgNum.emplace(321,8);
  pdgNum.emplace(323,9);
  pdgNum.emplace(331,10);
  pdgNum.emplace(333,11);
  pdgNum.emplace(1114,12);
  pdgNum.emplace(2112,13);
  pdgNum.emplace(2114,14);
  pdgNum.emplace(2212,15);
  pdgNum.emplace(2214,16);
  pdgNum.emplace(2212,15);
  pdgNum.emplace(2214,16);
  pdgNum.emplace(2224,17);
  pdgNum.emplace(3112,18);
  pdgNum.emplace(3114,19);
  pdgNum.emplace(3122,20);
  pdgNum.emplace(3212,21);
  pdgNum.emplace(3214,22);
  pdgNum.emplace(3222,23);
  pdgNum.emplace(3324,24);
  pdgNum.emplace(22,25);

  std::unordered_map<int,std::string>::iterator got;
  std::unordered_map<int,int>::iterator got2;
  for(int hh=0;hh<10000000;hh++) {
    got = pdgName.find(hh);
    if(got == pdgName.end()) pdgName.emplace(hh,"unknown");
    got2 = pdgNum.find(hh);
    if(got2 == pdgNum.end()) pdgNum.emplace(hh,25);
  }





  h_ak4_pt =  fs->make<TH1D>("h_ak4_pt"                ,"",100, 0,2000);
  h_njet=  fs->make<TH1D>("h_njet"                ,"",20, 0,20);
  h_ak4_genpt =  fs->make<TH1D>("h_ak4_genpt","",100, 0,2000);
  h_ak4_genpt_match =  fs->make<TH1D>("h_ak4_genpt_match","",100, 0,2000);
  h_ak4_eta            =  fs->make<TH1D>("h_ak4_eta"               ,"",100,-5,   5);
  h_ak4_phi            =  fs->make<TH1D>("h_ak4_phi"               ,"",100,-6,   6);
  h_nhf                = fs->make<TH1D>("h_nhf","",100,0,1.2);
  h_dr_jet             =  fs->make<TH1D>("h_dr_jet"                ,"",100, 0,6);
  h_dr_jet_all         =  fs->make<TH1D>("h_dr_jet_all"            ,"",100, 0,6);
  h_dr_jet_dRpt        =  fs->make<TH2D>("h_dr_jet_dRpt"            ,"",100, 0,6,100,0,1000);
  h_ak4_ndau           =  fs->make<TH1D>("h_ak4_ndau"              ,"",100, 0, 100);
  h_ak4_flavParton     =  fs->make<TH1D>("h_ak4_flavParton"        ,"",60,-30,30);
  h_ak4_flavHadron     =  fs->make<TH1D>("h_ak4_flavHadron"        ,"",60,-30,30);
  h_ak4_dRminGen       =  fs->make<TH1D>("h_ak4_dRminGen"          ,"",100, 0,  0.6);
  h_ak4_ptifmatchGen          =  fs->make<TH1D>("h_ak4_ptifmatchGen"             ,"",100, 0,2000);
  h_ak4_ptResponse     =  fs->make<TH1D>("h_ak4_ptResponse"        ,"",100,-2,   2);

  h_res_v_pt             =  fs->make<TH2D>("h_res_v_pt" ,"",50, 0,1000,100,0,2);
  h_pt1_pt2 = fs->make<TH2D>("h_pt1_pt2","",100,0,1000,100,0,1000);
  h_pt3   =  fs->make<TH1D>("h_pt3"        ,"",100,0,   1000);
  h_pt4   =  fs->make<TH1D>("h_pt4"        ,"",100,0,   1000);
  h_pt5   =  fs->make<TH1D>("h_pt5"        ,"",100,0,   1000);

  h_res_pt2550 = fs->make<TH1D>("h_res_pt2550","",100,0,2);
  h_res_pt5075 = fs->make<TH1D>("h_res_pt5075","",100,0,2);
  h_res_pt75100 = fs->make<TH1D>("h_res_pt75100","",100,0,2);
  h_res_pt100150 = fs->make<TH1D>("h_res_pt100150","",100,0,2);
  h_res_pt150200 = fs->make<TH1D>("h_res_pt150200","",100,0,2);
  h_res_pt200250 = fs->make<TH1D>("h_res_pt200250","",100,0,2);
  h_res_pt250300 = fs->make<TH1D>("h_res_pt250300","",100,0,2);
  h_res_pt300400 = fs->make<TH1D>("h_res_pt300400","",100,0,2);


  h_pt2550 = fs->make<TH1D>("h_pt2550","",100,25,50);
  h_pt5075 = fs->make<TH1D>("h_pt5075","",100,50,75);
  h_pt75100 = fs->make<TH1D>("h_pt75100","",100,75,100);
  h_pt100150 = fs->make<TH1D>("h_pt100150","",100,100,150);
  h_pt150200 = fs->make<TH1D>("h_pt150200","",100,150,200);
  h_pt200250 = fs->make<TH1D>("h_pt200250","",100,200,250);
  h_pt250300 = fs->make<TH1D>("h_pt250300","",100,250,300);
  h_pt300400 = fs->make<TH1D>("h_pt300400","",100,300,400);


  h_ak4_b_pt =  fs->make<TH1D>("h_ak4_b_pt","",100, 0,2000);
  h_ak4_bhgc_pt =  fs->make<TH1D>("h_ak4_bhgc_pt","",100, 0,2000);
  h_ak4_hgc_pt =  fs->make<TH1D>("h_ak4_hgc_pt","",100, 0,2000);
  h_ak4_hgchf_pt =  fs->make<TH1D>("h_ak4_hgchf_pt","",100, 0,2000);
  h_ak4_hf_pt =  fs->make<TH1D>("h_ak4_hf_pt","",100, 0,2000);


  h_ak4_b_res =  fs->make<TH1D>("h_ak4_b_res","",100, -1.2,5.);
  h_ak4_bhgc_res =  fs->make<TH1D>("h_ak4_bhgc_res","",100, -1.2,5.);
  h_ak4_hgc_res =  fs->make<TH1D>("h_ak4_hgc_res","",100, -1.2,5.);
  h_ak4_hgchf_res =  fs->make<TH1D>("h_ak4_hgchf_res","",100, -1.2,5.);
  h_ak4_hf_res =  fs->make<TH1D>("h_ak4_hf_res","",100, -1.2,5.);

  h_ak4_b_nhf = fs->make<TH1D>("h_ak4_b_nhf","",100,0.,1.0001);
  h_ak4_bhgc_nhf = fs->make<TH1D>("h_ak4_bhgc_nhf","",100,0.,1.0001);
  h_ak4_hgc_nhf = fs->make<TH1D>("h_ak4_hgc_nhf","",100,0.,1.0001);
  h_ak4_hgchf_nhf = fs->make<TH1D>("h_ak4_hgc_hf_nhf","",100,0.,1.0001);
  h_ak4_hf_nhf = fs->make<TH1D>("h_ak4_hf_nhf","",100,0.,1.0001);

  h_ak4_b_nemf = fs->make<TH1D>("h_ak4_b_nemf","",100,0.,1.0001);
  h_ak4_bhgc_nemf = fs->make<TH1D>("h_ak4_bhgc_nemf","",100,0.,1.0001);
  h_ak4_hgc_nemf = fs->make<TH1D>("h_ak4_hgc_nemf","",100,0.,1.0001);
  h_ak4_hgchf_nemf = fs->make<TH1D>("h_ak4_hgc_hf_nemf","",100,0.,1.0001);
  h_ak4_hf_nemf = fs->make<TH1D>("h_ak4_hf_nemf","",100,0.,1.0001);

  h_ak4_b_chf = fs->make<TH1D>("h_ak4_b_chf","",100,0.,1.0001);
  h_ak4_bhgc_chf = fs->make<TH1D>("h_ak4_bhgc_chf","",100,0.,1.0001);
  h_ak4_hgc_chf = fs->make<TH1D>("h_ak4_hgc_chf","",100,0.,1.0001);
  h_ak4_hgchf_chf = fs->make<TH1D>("h_ak4_hgc_hf_chf","",100,0.,1.0001);
  h_ak4_hf_chf = fs->make<TH1D>("h_ak4_hf_chf","",100,0.,1.0001);

  h_ak4_hgc_chfeta = fs->make<TH2D>("h_ak4_hgc_chfeta","",100,-5.,5.,100,0.,1.0001);

  h_ak4_nhfeta = fs->make<TH2D>("h_ak4_nhfeta","",100,-5.,5.,100,0.,1.0001);  
  h_ak4_nemfeta = fs->make<TH2D>("h_ak4_nemfeta","",100,-5.,5.,100,0.,1.0001);  
  h_ak4_chfeta = fs->make<TH2D>("h_ak4_chfeta","",100,-5.,5.,100,0.,1.0001);  
  h_ak4_cemfeta = fs->make<TH2D>("h_ak4_cemfeta","",100,-5.,5.,100,0.,1.0001);

  h_ak4_b_cemf = fs->make<TH1D>("h_ak4_b_cemf","",100,0.,1.0001);
  h_ak4_bhgc_cemf = fs->make<TH1D>("h_ak4_bhgc_cemf","",100,0.,1.0001);
  h_ak4_hgc_cemf = fs->make<TH1D>("h_ak4_hgc_cemf","",100,0.,1.0001);
  h_ak4_hgchf_cemf = fs->make<TH1D>("h_ak4_hgc_hf_cemf","",100,0.,1.0001);
  h_ak4_hf_cemf = fs->make<TH1D>("h_ak4_hf_cemf","",100,0.,1.0001);

  h_ineff_etaphi = fs->make<TH2D>("h_ineff_etaphi","",100,-5.,5.,100,-4.,4.);
  h_ineff_etapt = fs->make<TH2D>("h_ineff_etapt","",100,-5.,5.,100,0.,1000.);


  ak4JetTree = new TTree("ak4JetTree","ak4JetTree");  
  ak4JetTree->Branch("ak4_pt"               , & ak4_pt             , "ak4_pt/F"               );          
  ak4JetTree->Branch("ak4_eta"              , & ak4_eta            , "ak4_eta/F"              );          
  ak4JetTree->Branch("ak4_ptGen"            , & ak4_ptGen          , "ak4_ptGen/F"            );          
  ak4JetTree->Branch("ak4_ptResponse"       , & ak4_ptResponse     , "ak4_ptResponse/F"       );          
  ak4JetTree->Branch("ak4_dRminGen"         , & ak4_dRminGen       , "ak4_dRminGen/F"         );          
  ak4JetTree->Branch("ak4_area"             , & ak4_area           , "ak4_area/F"             );          

  consumes<reco::GenParticleCollection> (edm::InputTag("genParticles"));
  consumes<reco::GenJetCollection> (edm::InputTag("ak4GenJets"));

 
}


JetMiniValidation::~JetMiniValidation()
{
}


//
// member functions
//

// ------------ method called for each event  ------------
void
JetMiniValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;
  using namespace pat;

  bool verbose = true;

  rhtools_.getEventSetup(iSetup);

  if(verbose) std::cout<<"new event"<<std::endl;

  
  edm::Handle<reco::PFJetCollection> AK4PFJET;
  iEvent.getByToken(ak4jetToken_, AK4PFJET);

  edm::Handle<reco::GenJetCollection> AK4GENJET;  
  iEvent.getByToken(ak4genjetToken_, AK4GENJET);

  edm::Handle<reco::GenParticleCollection> genParticlesH_;
  iEvent.getByLabel("genParticles", genParticlesH_);  


  edm::Handle<edm::PCaloHitContainer> PCaloHits;

  // gen particles
  if(verbose) std::cout<<" starting gen particles"<<std::endl;
  int icntg=0;
  for (reco::GenParticleCollection::const_iterator  igen = genParticlesH_->begin()\
	 ; igen != genParticlesH_->end(); igen++) {
    int iid = (*igen).pdgId();
    if(verbose) std::cout<<"  gen part pt["<<icntg<<"]="<<(*igen).pt()<<std::endl;
    icntg++;
  }
  if(verbose)  std::cout<<" number of gen particles is "<<icntg<<std::endl;
  
  //****************************************************
  // gen jets
  //****************************************************
  int iigen=-1;
  if(verbose) std::cout<<" starting gen jets"<<std::endl;
  for (const reco::GenJet &igen : *AK4GENJET) {  
    iigen++;
    double pt = igen.pt();
    double eta = igen.eta();
    double phi = igen.phi();
    double e = igen.energy();

    double EME       = igen.emEnergy();
    double HE       = igen.hadEnergy();
    double IE       = igen.invisibleEnergy();
    int iConst = igen.nConstituents();

    if(verbose) { 
      if(pt>50&&e>0) {
	std::cout<<"  "<<iigen<<" "<<pt<<" "<<e<<" "<<eta<<" "<<phi<<" "<<EME<<" "<<HE<<" "<<IE<<" "<<(EME+HE+IE)/e<<" "<<iConst<<std::endl;
	std::cout<<"   with constituents:"<<std::endl;
	std::vector<const GenParticle*> mcparts = igen.getGenConstituents();
	for(unsigned i=0; i<mcparts.size(); i++ ) {
	  const GenParticle* mcpart = mcparts[i];
	  if(mcpart->pt()>1) {
	    std::cout<<"       "<<partNames[pdgNum[abs(mcpart->pdgId())]]<<" "<<mcpart->pdgId()<<" "<<mcpart->pt()<<std::endl;
	  }
	}
      }
    }



    if(pt>50) h_ak4_genpt->Fill(pt);
    // find gen jets with no reco jet match
    if(pt>50) {
      float dRmin =100.;
      for (const reco::PFJet &ijet : *AK4PFJET) {  
        double ptr           = ijet.pt();
        double etar          = ijet.eta();

        // jet ID
        double NHF       = ijet.neutralHadronEnergyFraction();
        double NEMF      = ijet.neutralEmEnergyFraction();
        double CHF       = ijet.chargedHadronEnergyFraction();
        double CEMF      = ijet.chargedEmEnergyFraction();
        double NM        = ijet.neutralMultiplicity();
        double CM        = ijet.chargedMultiplicity(); 
        bool goodJet_looseJetID =  
          (fabs(etar) < 2.4 && CHF > 0.0 && NHF < 0.99 && CM > 0 && CEMF < 0.99 && NEMF < 0.99) 
          || ( fabs(etar) >= 2.4 && fabs(etar) < 3.0 && NEMF < 0.9 && NM > 2 ) 
          || ( fabs(etar) >= 3.0 && NEMF < 0.9 && NM > 10 );
	//goodJet_looseJetID = true;
	
	if((ptr>20)&&goodJet_looseJetID) {
          float dR = reco::deltaR(ijet.eta(),ijet.phi(),igen.eta(),igen.phi());
          if (dR < dRmin) {
            dRmin = dR;
          }
	}


      }  // end loop over reco jets

      if(dRmin>0.4) {
        h_ineff_etaphi->Fill(eta,phi);
        h_ineff_etapt->Fill(eta,pt);
        if(verbose) {
	  std::cout<<" gen jet "<<iigen<<" has no matching reco jet"<<std::endl;
        }
      }
      else {
	h_ak4_genpt_match->Fill(pt);
      }

    }  // end pt cut on gen jet

  }  // end loop over gen jet

  //****************************************************
  // rec hits
  //****************************************************************

  edm::Handle<HGCRecHitCollection> ee_hits;
  edm::Handle<HGCRecHitCollection> fh_hits;
  edm::Handle<HGCRecHitCollection> bh_hits;
  iEvent.getByToken(hits_ee_token,ee_hits);
  iEvent.getByToken(hits_fh_token,fh_hits);
  iEvent.getByToken(hits_bh_token,bh_hits);


  if(verbose)  std::cout<<" starting rec hits"<<std::endl;  
  for (unsigned int i=0;i<(*ee_hits).size();++i) {
    const HGCRecHit& hgrh = (*ee_hits)[i];
    double anenergy = hgrh.energy();
    DetId detid = hgrh.detid();
    unsigned int layer = rhtools_.getLayerWithOffset(detid);
  const GlobalPoint position( std::move( rhtools_.getPosition( detid ) ) );
  }
  
  for (unsigned int i=0;i<(*fh_hits).size();++i) {
    const HGCRecHit& hgrh = (*fh_hits)[i];
    double anenergy = hgrh.energy();
    DetId detid = hgrh.detid();
    unsigned int layer = rhtools_.getLayerWithOffset(detid);
    const GlobalPoint position( std::move( rhtools_.getPosition( detid ) ) );
  }
  
  for (unsigned int i=0;i<(*bh_hits).size();++i) {
    const HGCRecHit& hgrh = (*bh_hits)[i];
    double anenergy = hgrh.energy();
    DetId detid = hgrh.detid();
    unsigned int layer = rhtools_.getLayerWithOffset(detid);
  const GlobalPoint position( std::move( rhtools_.getPosition( detid ) ) );
  }
  

  //****************************************************
  // sim hits
  //****************************************************


  if(verbose) std::cout<<" starting sim hits"<<std::endl;
  for( typename std::vector<edm::EDGetTokenT<edm::PCaloHitContainer> >::const_iterator
	 token = m_PCaloHitsTokens.begin(); token != m_PCaloHitsTokens.end(); ++token ) {
    unsigned index(token - m_PCaloHitsTokens.begin());

    //
    // Geometry & looping over rechits
    //                           
    std::string nameDetector_ = m_geometrySource[index];
    //std::cout << nameDetector_ << std::endl;

    //
    // getByToken
    // 
    iEvent.getByToken(*token, PCaloHits);
    if( PCaloHits.isValid() && !PCaloHits->empty() ) {
      //std::cout << "Input found" << std::endl;
      edm::LogInfo("Input found") << m_PCaloHitsTags.at(index);
    } else {
      //std::cout << "Input not found" << std::endl;
      edm::LogInfo("Input not found") << m_PCaloHitsTags.at(index);
      continue;
    }
    std::cout << nameDetector_ << " " << PCaloHits->size() << std::endl;

    //
    // Loop over PCaloHits
    //
    for (const auto & it : *(PCaloHits.product())) {  
      

      int    cell, sector, layer, zside;
      int subsector;
      int    subdet(0);
      HepGeom::Point3D<float> gcoord;
      unsigned int id_ = it.id();

      // 
      if (nameDetector_ == "HCal") {
	//std::cout<<"HCAL"<<std::endl;
	//
	int z, depth, eta, phi, lay;
	HcalTestNumbering::unpackHcalIndex(it.id(), subdet, z, depth, eta, phi, lay);
	//std::cout<<subdet<<" "<<z<<" "<<depth<<" "<<eta<<" "<<phi<<" "<<lay<<std::endl;
	if (subdet != static_cast<int>(HcalEndcap)) continue;
	//std::cout<<"HCAL endcap"<<std::endl;
	
	HcalCellType::HcalCell hccell = hcCons_->cell(subdet, z, lay, eta, phi);
	
	double zp  = hccell.rz/10;  // mm -> cm, rz is actually Z?
	
	int sign = (z==0)?(-1):(1);
	zp      *= sign;
	double rho = zp*tan(2.0*atan(exp(-hccell.eta)));
	double xp  = rho * cos(hccell.phi); //cm
	double yp  = rho * sin(hccell.phi); //cm
	//std::cout<<zp<<" "<<rho<<" "<<xp<<" "<<yp<<std::endl;
	
	HcalDetId detId = HcalHitRelabeller::relabel(id_,hcConr_);
	subdet           = detId.subdet();
	if (subdet != static_cast<int>(HcalEndcap)) continue;
	cell             = detId.ietaAbs();
	sector           = detId.iphi();
	//subsector        = 1;
	layer            = detId.depth();
	zside            = detId.zside();

	//std::cout << it.energy() << " " << subdet << " "<<subsector<<std::endl;

	if (it.energy()>0.5) std::cout << "HcalTupleMaker_HGCSimHits: " 
				       << it.energy() << " " 
				       << nameDetector_ << " " 
				       << subdet << " " << cell << " " << sector << std::endl;

	std::pair<double,double> etaphi = hcConr_->getEtaPhi(subdet,zside*cell,sector);
	double rz = hcConr_->getRZ(subdet,zside*cell,layer);  // This is actually Z?
	  
	gcoord = HepGeom::Point3D<float>(rz*cos(etaphi.second)/cosh(etaphi.first)/tanh(etaphi.first),
					 rz*sin(etaphi.second)/cosh(etaphi.first)/tanh(etaphi.first),
					 rz);

	//
	// Use CaloCellGeometry getPosition
	// 
	const CaloCellGeometry* cellGeometry = hcGeometry_->getGeometry(detId);
	/*
	std::cout << "HCAL geom comparison: "
		  << "(" << xp         << ", " << yp         << ", " << zp         << ") "  
		  << rho << " "
		  << "(" << gcoord.x() << ", " << gcoord.y() << ", " << gcoord.z() << ") "  
		  << "(" << cellGeometry->getPosition().x() << ", " << cellGeometry->getPosition().y() << ", " << cellGeometry->getPosition().z() << ") "  
		  << std::endl;
	*/

	//
	// Use CaloCellGeometry getPosition() method at the end
	// 
	gcoord = HepGeom::Point3D<float>(cellGeometry->getPosition().x(),
					 cellGeometry->getPosition().y(),
					 cellGeometry->getPosition().z());


	
      } 
      
      else {
	
	if (it.energy()>0.5) std::cout << "Will Robinson  HcalTupleMaker_HGCSimHits: " <<
			       nameDetector_<<" "
				       << it.energy() << std::endl;
	
	  
	std::cout << "HcalTupleMaker_HGCSimHits: " << hgcCons_[index]->geomMode() << std::endl;
	
	if (hgcCons_[index]->geomMode() == HGCalGeometryMode::Square) {
	  
	  
	  std::cout << "HcalTupleMaker_HGCSimHits: in the square mode." << std::endl;
	  HGCalTestNumbering::unpackSquareIndex(id_, zside, layer, sector, subsector, cell);
	  std::pair<float,float> xy = hgcCons_[index]->locateCell(cell,layer,subsector,false);
	  const HepGeom::Point3D<float> lcoord(xy.first,xy.second,0);
	  bool symmDet_=true;
	  int subs = (symmDet_ ? 0 : subsector);
	  id_      = HGCalTestNumbering::packSquareIndex(zside,layer,sector,subs,0);
	  gcoord   = (transMap_[id_]*lcoord); // 
	  

	} else {

	  double zp, xp, yp;
	  std::pair<float,float> xy = hgcCons_[index]->locateCell(cell,layer,sector,false); //mm
          zp = hgcCons_[index]->waferZ(layer,false); //cm 
          //if (zside < 0) zp = -zp;
          xp = (zp<0) ? -xy.first/10 : xy.first/10; //mm
          yp = xy.second/10; //mm  
	  std::cout<<"will rob xp yp "<<xp<<" "<<yp<<std::endl;

	} // end geometry test
	
      }  // end not hcal
	
    }  // end loop over hits
  }  // end loop over detectors


  //****************************************************
  // reco jets
  // ********************************************
  int igood=0.;
  float pt1=0.;
  float pt2=0.;
  float pt3=0.;
  float pt4=0.;
  float pt5=0.;
  int iijet=-1;
  if(verbose) std::cout<<" starting reco jets "<<std::endl;
  for (const reco::PFJet &ijet : *AK4PFJET) {  
    iijet++;
    double pt = ijet.pt();
    double eta = ijet.eta();
    double phi = ijet.phi();
    double ndau = ijet.numberOfDaughters();
    int iConst = ijet.nConstituents();

    if(verbose) std::cout<<"   reco jet["<<iijet<<"] pt="<<pt<<std::endl;
    // jet ID
    double NHF       = ijet.neutralHadronEnergyFraction();
    double NEMF      = ijet.neutralEmEnergyFraction();
    double CHF       = ijet.chargedHadronEnergyFraction();
    double CEMF      = ijet.chargedEmEnergyFraction();
    double NM        = ijet.neutralMultiplicity();
    double CM        = ijet.chargedMultiplicity(); 

    bool goodJet_looseJetID =  
      (fabs(eta) < 2.4 && CHF > 0.0 && NHF < 0.99 && CM > 0 && CEMF < 0.99 && NEMF < 0.99) 
      || ( fabs(eta) >= 2.4 && fabs(eta) < 3.0 && NEMF < 0.9 && NM > 2 ) 
      || ( fabs(eta) >= 3.0 && NEMF < 0.9 && NM > 10 );
    if (verbose) if(pt>50) cout<<" "<<iijet<<" "<<pt<<" "<<eta<<" "<<phi<<" "<<"goodJet = "<<goodJet_looseJetID<<" CHF "<<CHF<<" NHF "<<NHF<<" CEMF "<<CEMF<<" NEMF "<<NEMF<<" CM "<<CM<<" NM "<<NM<<" ndau "<<ndau<<" iConst "<<iConst<<endl;


    // find delta_R between this jet and nearest neighbor
    float dRjet =100.;
    int imatch=-1;
    if(ijet.pt()>50) {
      int kkjet=-1;
      for (const reco::PFJet &kjet : *AK4PFJET) {  
	kkjet++;
        if(kjet.pt()>50) {
          float dR = reco::deltaR(ijet.eta(),ijet.phi(),kjet.eta(),kjet.phi());
	  if(dR>0.1) {
            h_dr_jet_all->Fill(dR);
            h_dr_jet_dRpt->Fill(dR,kjet.pt());
            if (dR < dRjet) {
	      dRjet=dR;
	      imatch=kkjet;
	    }
	  }
        }
      }
      h_dr_jet->Fill(dRjet);
    }
    if(verbose) {
      if(pt>50)
        std::cout<<"   jet "<<iijet<<" matches jet "<<imatch<<" with dr of "<<dRjet<<std::endl;
    }

    if((pt>50)&&(goodJet_looseJetID)) {
      if(verbose) { 
	std::cout<<"    in with pt eta "<<pt<<" "<<eta<<std::endl;

	std::vector<reco::PFCandidatePtr> candss = ijet.getPFConstituents();
	for (unsigned ic = 0; ic < candss.size (); ++ic) {
	  std::cout<<"    constituent with pt of "<<candss[ic]->pt()<<" and id of "<<candss[ic]->pdgId()<<std::endl;
	}

      }


      igood+=1;
      if(igood==1) pt1=pt;
      if(igood==2) pt2=pt;
      if(igood==3) pt3=pt;
      if(igood==4) pt4=pt;
      if(igood==5) pt5=pt;
      if(verbose) std::cout<<"    "<<iijet<<" counted as igood "<<igood<<" "<<std::endl;

      h_ak4_pt         ->Fill( pt         );
      h_ak4_eta        ->Fill( eta        );
      h_ak4_phi        ->Fill( phi        );
      h_ak4_ndau       ->Fill( ndau       );
      h_nhf            ->Fill(NHF);    


      float dRmin = 100.;
      float ptGen = -1.0;
      iigen=0;
      int imatchgen=-1;
      for (const reco::GenJet &igen : *AK4GENJET) {  
	iigen++;
        float dR = reco::deltaR(ijet.eta(),ijet.phi(),igen.eta(),igen.phi());
        if (dR < dRmin) {
          dRmin = dR;
          ptGen = igen.pt();
	  imatchgen=iigen;
        }
      }
      float ptres = -1.;
      if(dRmin>0.4) {  // no matching gen jet
	ptGen=-1.0;
	imatchgen=0;
      } else {  // matching gen jet
	ptres=pt/ptGen;
      }
      if(verbose) std::cout<<"    jet "<<iijet<<" matches gen jet "<<imatchgen<<std::endl;
      h_ak4_dRminGen         ->Fill( dRmin       );
      if (ptGen>0) {
	h_ak4_ptResponse       ->Fill( ptres );
        h_ak4_ptifmatchGen            ->Fill( pt  );
      }

      h_res_v_pt       ->Fill(ptGen,ptres);

      if(ptGen>25&&ptGen<50) {h_res_pt2550->Fill(ptres); h_pt2550->Fill(ptGen);}
      if(ptGen>50&&ptGen<75) {h_res_pt5075->Fill(ptres); h_pt5075->Fill(ptGen);}
      if(ptGen>75&&ptGen<100) {h_res_pt75100->Fill(ptres); h_pt75100->Fill(ptGen);}
      if(ptGen>100&&ptGen<150) {h_res_pt100150->Fill(ptres); h_pt100150->Fill(ptGen);}
      if(ptGen>105&&ptGen<200) {h_res_pt150200->Fill(ptres); h_pt150200->Fill(ptGen);}
      if(ptGen>200&&ptGen<250) {h_res_pt200250->Fill(ptres); h_pt200250->Fill(ptGen);}
      if(ptGen>250&&ptGen<300) {h_res_pt250300->Fill(ptres); h_pt250300->Fill(ptGen);}
      if(ptGen>300&&ptGen<400) {h_res_pt300400->Fill(ptres); h_pt300400->Fill(ptGen);}


      h_ak4_nhfeta->Fill(eta,NHF);
      h_ak4_nemfeta->Fill(eta,NEMF);
      h_ak4_chfeta->Fill(eta,CHF);
      h_ak4_cemfeta->Fill(eta,CEMF);


      // histograms by region
      if(fabs(eta)<1.1) {  // barrel only
        h_ak4_b_pt ->Fill( pt         );
        h_ak4_b_res ->Fill( ptres );
	h_ak4_b_nhf->Fill(NHF);
	h_ak4_b_nemf->Fill(NEMF);
	h_ak4_b_chf->Fill(CHF);
	h_ak4_b_cemf->Fill(CEMF);
      }
      if((fabs(eta)>1.1)&&(fabs(eta)<1.9)) { // barrel-hgc overlap
        h_ak4_bhgc_pt         ->Fill( pt         );
        h_ak4_bhgc_res ->Fill( ptres );
	h_ak4_bhgc_nhf->Fill(NHF);
	h_ak4_bhgc_nemf->Fill(NEMF);
	h_ak4_bhgc_chf->Fill(CHF);
	h_ak4_bhgc_cemf->Fill(CEMF);

      }
      if((fabs(eta)>1.9)&&(fabs(eta)<2.6)) {  // hgc only
	if(verbose) {
	  std::cout<<"   jet "<<iijet<<" is hgc jet with pt ptres of "<<pt<<" "<<ptres
		   <<std::endl;
	}
        h_ak4_hgc_pt         ->Fill( pt         );
        h_ak4_hgc_res ->Fill( ptres );
	h_ak4_hgc_nhf->Fill(NHF);
	h_ak4_hgc_nemf->Fill(NEMF);
	h_ak4_hgc_chf->Fill(CHF);
	h_ak4_hgc_chfeta->Fill(eta,CHF);
	h_ak4_hgc_cemf->Fill(CEMF);

      }
      if((fabs(eta)>2.6)&&(fabs(eta)<3.4)) { // hgc hf overlap
        h_ak4_hgchf_pt         ->Fill( pt         );
        h_ak4_hgchf_res ->Fill( ptres );
	h_ak4_hgchf_nhf->Fill(NHF);
	h_ak4_hgchf_nemf->Fill(NEMF);
	h_ak4_hgchf_chf->Fill(CHF);
	h_ak4_hgchf_cemf->Fill(CEMF);

      }
      if(fabs(eta)>3.4) {  // hf only
        h_ak4_hf_pt         ->Fill( pt         );
        h_ak4_hf_res ->Fill( ptres );
	h_ak4_hf_nhf->Fill(NHF);
	h_ak4_hf_nemf->Fill(NEMF);
	h_ak4_hf_chf->Fill(CHF);
	h_ak4_hf_cemf->Fill(CEMF);

      }
     
   
      ak4_ptGen       =  ptGen;
      ak4_ptResponse  =  ptres ;
      ak4_dRminGen    =  dRmin;
      ak4_pt          =  pt;
      ak4_eta         =  eta;
      ak4_area     = ijet.jetArea();

      ak4JetTree->Fill();

    }  // end pt and id cut on selected jets

  }  //end loop over jets
  h_pt1_pt2->Fill(pt1,pt2);
  h_pt3->Fill(pt3);
  h_pt4->Fill(pt4);
  h_pt5->Fill(pt5);
  h_njet->Fill(igood);

  if(verbose) std::cout<<std::endl<<std::endl;
}


// ------------ method called once each job just before starting event loop  ------------
void 
JetMiniValidation::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetMiniValidation::endJob() 
{
}

void
JetMiniValidation::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
{


  //initiating HGC Geometry
  for (size_t i=0; i<m_geometrySource.size(); i++) {
      
    // HCAL for BH/HEB
    if (m_geometrySource[i].find("HCal") != std::string::npos) {
      edm::ESHandle<HcalDDDSimConstants> pHSNDC;
      iSetup.get<HcalSimNumberingRecord>().get(pHSNDC);
      if (pHSNDC.isValid()) {
	hcCons_ = pHSNDC.product();
	hgcCons_.push_back(0);
      } else {
	edm::LogWarning("HGCalValid") << "Cannot initiate HcalDDDSimConstants: "
				      << m_geometrySource[i] << std::endl;
      }
      edm::ESHandle<HcalDDDRecConstants> pHRNDC;
      iSetup.get<HcalRecNumberingRecord>().get(pHRNDC);
      if (pHRNDC.isValid()) {
	hcConr_ = pHRNDC.product();
      } else {
	edm::LogWarning("HGCalValid") << "Cannot initiate HcalDDDRecConstants: "
				      << m_geometrySource[i] << std::endl;
      }
      edm::ESHandle<CaloGeometry> caloG;
      iSetup.get<CaloGeometryRecord>().get(caloG);
      if (caloG.isValid()) {
	const CaloGeometry* geo = caloG.product();
	hcGeometry_ = geo->getSubdetectorGeometry(DetId::Hcal,HcalBarrel);
	hgcGeometry_.push_back(0);
      } else {
	edm::LogWarning("HGCalValid") << "Cannot initiate HcalGeometry for "
				      << m_geometrySource[i] << std::endl;
      }
    }
    // HGC for EE & HEF
    else {
      edm::ESHandle<HGCalDDDConstants> hgcCons;
      iSetup.get<IdealGeometryRecord>().get(m_geometrySource[i],hgcCons);
      if (hgcCons.isValid()) {
	hgcCons_.push_back(hgcCons.product());
      } else {
	edm::LogWarning("HGCalValid") << "Cannot initiate HGCalDDDConstants for "
				      << m_geometrySource[i] << std::endl;
      }
      edm::ESHandle<HGCalGeometry> hgcGeom;
      iSetup.get<IdealGeometryRecord>().get(m_geometrySource[i],hgcGeom);
      if(hgcGeom.isValid()) {
	hgcGeometry_.push_back(hgcGeom.product());
      } else {
	edm::LogWarning("HGCalValid") << "Cannot initiate HGCalGeometry for "
				      << m_geometrySource[i] << std::endl;
      }
    }
  }

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetMiniValidation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetMiniValidation);
