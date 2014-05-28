import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'PRE_STA71_V4::All'


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/6620DF7E-4DE3-E311-B6E1-02163E00EBB7.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/8E9EB9B3-89E2-E311-8582-02163E00E730.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/A4EDB78C-89E2-E311-BC5E-02163E00E670.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/B2B2AB97-4DE3-E311-975B-02163E00EB25.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/C030C4B7-89E2-E311-9B6C-02163E00E761.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/C8CAC893-89E2-E311-9FAE-02163E00EA30.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/E8C09FAB-89E2-E311-BF01-02163E00E6CE.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/FCCF6FE5-89E2-E311-AE27-02163E00EB68.root",
                                      "/store/relval/CMSSW_7_1_0_pre8/RelValTTbarLepton/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_STA71_V4-v1/00000/FE6FEA77-8AE2-E311-B1CB-02163E00BC16.root"
                                      )
)

process.matchingAnalyzer = cms.EDAnalyzer('MatchWithTriggerObj'
)
process.TFileService = cms.Service( "TFileService",
                                   fileName = cms.string( 'filesWithHistos.root' )
                                   )

process.p = cms.Path(process.matchingAnalyzer)
