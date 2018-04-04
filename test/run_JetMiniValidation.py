import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.parseArguments()

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.Geometry.GeometryExtended2023D17Reco_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        options.inputFiles
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_3.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_5.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_6.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_7.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_8.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_9.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_10.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_11.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_12.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_13.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_14.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_15.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_16.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_17.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_18.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_18.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_20.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_21.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_22.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_23.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_24.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_25.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_26.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_27.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_28.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_29.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-scenario3-step3-HcalEcal_aged-SiPM_nonaged/170726_130015/0000/step3_30.root',
#'file:/data/users/eno/simple/CMSSW_9_1_0_pre3/src/FED2618B-1331-E711-819D-0025905B85C0.root '
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_1.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_2.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_3.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_4.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_5.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_7.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_8.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_9.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_11.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_15.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_16.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_17.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_18.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_19.root',
#'file:/mnt/hadoop/cms/store/user/snabili/RelValQCD_Pt-15To7000_Flat_14TeV/Data_analysis-step3-HcalEcalSiPM-aged/170725_131733/0000/step3_20.root',

    )
)

process.ana = cms.EDAnalyzer('JetMiniValidation',
          jecPayloads = cms.vstring([
                                    '../data/Spring16_25nsV5_MC/Spring16_25nsV5_MC_L1FastJet_AK8PFchs.txt',
                                    '../data/Spring16_25nsV5_MC/Spring16_25nsV5_MC_L2Relative_AK8PFchs.txt',
                                    '../data/Spring16_25nsV5_MC/Spring16_25nsV5_MC_L3Absolute_AK8PFchs.txt',
                                    '../data/Spring16_25nsV5_MC/Spring16_25nsV5_MC_L2L3Residual_AK8PFchs.txt',
                                    '../data/Spring16_25nsV5_MC/Spring16_25nsV5_MC_Uncertainty_AK8PFchs.txt'
                                    ]),
source = cms.untracked.VInputTag(
        cms.untracked.InputTag("g4SimHits","HGCHitsEE"),
        cms.untracked.InputTag("g4SimHits","HGCHitsHEfront"),
        cms.untracked.InputTag("g4SimHits","HcalHits")
        ),
  geometrySource = cms.untracked.vstring(
        'HGCalEESensitive',
        'HGCalHESiliconSensitive',
        'HCal'
  ),
  Prefix = cms.untracked.string  ("HGCSimHits"),
  Suffix = cms.untracked.string  ("")
)


### OUT
process.TFileService = cms.Service("TFileService",
      fileName = cms.string("out.root"),
      closeFileFast = cms.untracked.bool(True)
  )

process.p = cms.Path(process.ana)
