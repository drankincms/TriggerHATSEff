#include "MatchWithTriggerObj.h"


//
MatchWithTriggerObj::MatchWithTriggerObj(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


MatchWithTriggerObj::~MatchWithTriggerObj()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MatchWithTriggerObj::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;

//testing git

    edm::Handle<GenEventInfoProduct> genEvent;
    iEvent.getByLabel("generator", genEvent);

    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel( "genParticles", genParticles );
    
    edm::Handle<reco::GenJetCollection> ak5GenJets;
    iEvent.getByLabel( "ak5GenJets", ak5GenJets );
    
    bool changedConfig = false;
    if (!hltConfig.init(iEvent.getRun(), iSetup, "HLT", changedConfig)) {
        cout << "Initialization of HLTConfigProvider failed!!" << endl;
        return;
    }
    if (changedConfig){
        std::cout << "the curent menu is " << hltConfig.tableName() << std::endl;
        triggerBitMu = -1;
        triggerBitMu3Jets = -1;
        for (size_t j = 0; j < hltConfig.triggerNames().size(); j++) {
            if (TString(hltConfig.triggerNames()[j]).Contains("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v")) triggerBitMu3Jets = j;
            if (TString(hltConfig.triggerNames()[j]).Contains("HLT_IsoMu24_v")) triggerBitMu = j;
        }
        if (triggerBitMu == -1 || triggerBitMu3Jets) cout << "HLT path not found" << endl;
       
    }
    
    edm::InputTag triggerResultsLabel = edm::InputTag("TriggerResults", "", "HLT");
    edm::Handle<edm::TriggerResults> triggerResults;
    iEvent.getByLabel(triggerResultsLabel, triggerResults);
    
    
    

    int theNbOfGen = genParticles->size();
    for (int i=0 ; i < theNbOfGen; i++){
        const reco::GenParticle & genMuon = (*genParticles)[i];
        if ((fabs(genMuon.pdgId())==13)&&(genMuon.status()==1)){
            if ((fabs(genMuon.eta())>2.4)) continue;
            histos1D_[ "ptDenom" ]->Fill(genMuon.pt());
            if (triggerResults->accept(triggerBitMu)) histos1D_[ "ptPassing" ]->Fill(genMuon.pt(),1);
            else histos1D_[ "ptPassing" ]->Fill(genMuon.pt(),0);
            
        }
    }
    
    int theNbofGenJets = ak5GenJets->size();
    double leadPT=0;
    double subleadPT;
    for (int i=0 ; i < theNbOfGen; i++){
        const reco::GenParticle & genMuon = (*genParticles)[i];
        if ((fabs(genMuon.pdgId())==13)&&(genMuon.status()==1)&&theNbofGenJets>=3){
            if ((fabs(genMuon.eta())>2.4)) continue;
            for (int j=0 ; j < theNbofGenJets ; j++){
                const reco::GenJet & genJet = (*ak5GenJets)[j];
                if ((genJet.pt())>leadPT) {
                    subleadPT=leadPT;
                    leadPT=genJet.pt();
                }
                else if ((genJet.pt())>subleadPT) subleadPT=genJet.pt();
            }
            histos2D_[ "ptDenom" ]->Fill(genMuon.pt(),leadPT);
            if (triggerResults->accept(triggerBitMu3Jets)) histos2D_[ "ptPassing" ]->Fill(genMuon.pt(),leadPT,1);
            else histos2D_[ "ptPassing" ]->Fill(genMuon.pt(),leadPT,0);
        }
    }

}


// ------------ method called once each job just before starting event loop  ------------
void 
MatchWithTriggerObj::beginJob()
{
    edm::Service< TFileService > fileService;
    histos1D_[ "ptDenom" ] = fileService->make< TH1D >( "ptDenom", "nb of muon pair", 10, 0., 100);
    histos1D_[ "ptDenom" ]->SetXTitle( "muon p_{T} (GeV)" );
    
    histos1D_[ "ptPassing" ] = fileService->make< TH1D >( "ptPassing", "nb of muon passing path", 10, 0., 100);
    histos1D_[ "ptPassing" ]->SetXTitle( "muon p_{T} (GeV)" );
    
    histos1D_[ "pt1Defficiency" ] = fileService->make< TH1D >( "pt1Defficiency", "efficiency", 10, 0., 100);
    histos1D_[ "pt1Defficiency" ]->SetXTitle( "muon p_{T} (GeV)" );
    
    histos2D_[ "ptDenom" ] = fileService->make< TH2D >( "ptDenom", "nb of muon+3jets", 10, 0., 100, 10, 0., 100);
    histos2D_[ "ptDenom" ]->SetXTitle( "muon p_{T} (GeV)" );
    histos2D_[ "ptDenom" ]->SetYTitle( "lead jet p_{T} (GeV)" );
    
    histos2D_[ "ptPassing" ] = fileService->make< TH2D >( "ptPassing", "nb of muon+3jets passing path", 10, 0., 100, 10, 0., 100);
    histos2D_[ "ptPassing" ]->SetXTitle( "muon p_{T} (GeV)" );
    histos2D_[ "ptPassing" ]->SetYTitle( "lead jet p_{T} (GeV)" );
    
    histos2D_[ "pt2Defficiency" ] = fileService->make< TH2D >( "pt2Defficiency", "efficiency", 10, 0., 100, 10, 0., 100);
    histos2D_[ "pt2Defficiency" ]->SetXTitle( "muon p_{T} (GeV)" );
    histos2D_[ "pt2Defficiency" ]->SetYTitle( "lead jet p_{T} (GeV)" );

}

// ------------ method called once each job just after ending the event loop  ------------
void 
MatchWithTriggerObj::endJob()
{

    histos2D_[ "pt2Defficiency" ]->Sumw2();
    histos2D_[ "pt2Defficiency" ]->Divide(histos2D_[ "ptPassing" ],histos2D_[ "ptDenom" ],1,1);

    histos1D_[ "pt1Defficiency" ]->Sumw2();
    histos1D_[ "pt1Defficiency" ]->Divide(histos1D_[ "ptPassing" ],histos1D_[ "ptDenom" ]);
    
  }


// ------------ method called when starting to processes a run  ------------
/*
void 
MatchWithTriggerObj::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MatchWithTriggerObj::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MatchWithTriggerObj::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MatchWithTriggerObj::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MatchWithTriggerObj::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

}

//define this as a plug-in
DEFINE_FWK_MODULE(MatchWithTriggerObj);
