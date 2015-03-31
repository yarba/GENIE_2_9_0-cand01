//____________________________________________________________________________
/*
 Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Chris Barry <cbarry \at hep.ph.liv.ac.uk>
         University of Liverpool

	 Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Laboratory
 
 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TIterator.h>
#include <TH1D.h>

#include "EVGCore/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "Ntuple/NtpMCFormat.h"
#include "Ntuple/NtpMCTreeHeader.h"
#include "Ntuple/NtpMCEventRecord.h"
#include "Messenger/Messenger.h"
#include "PDG/PDGCodes.h"
#include "Utils/CmdLnArgParser.h"

#include "DataComp/MiniBooNE/MBCC1pi0Model.h"
#include "DataComp/MiniBooNE/MBConstants.h"

using namespace genie;
using namespace genie::mc_vs_data;
//using namespace genie::mc_vs_data::constants;
using namespace genie::mc_vs_data::constants::MB;

//____________________________________________________________________________
MBCC1pi0Model::MBCC1pi0Model(MBCC1pi0Binning & binning) 
{
  // Initialise arrays

  Initialise( fEnuXsec        , CC1pi0::kNEnuBins         );
  Initialise( fQ2Xsec         , CC1pi0::kNQ2Bins          );
  Initialise( fEmuXsec        , CC1pi0::kNEmuBins         );
  Initialise( fCosThetamuXsec , CC1pi0::kNCosThetamuBins  );
  Initialise( fPpi0Xsec       , CC1pi0::kNPpi0Bins        );
  Initialise( fCosThetapi0Xsec, CC1pi0::kNCosThetapi0Bins );

  // Set pointers to bin edges arrays
  
  fEnuBinEdges         = binning.GetEnuBinEdges()         ;
  fQ2BinEdges          = binning.GetQ2BinEdges()          ;
  fEmuBinEdges         = binning.GetEmuBinEdges()         ;
  fCosThetamuBinEdges  = binning.GetCosThetamuBinEdges()  ;
  fPpi0BinEdges        = binning.GetPpi0BinEdges()        ;
  fCosThetapi0BinEdges = binning.GetCosThetapi0BinEdges() ;

}
//____________________________________________________________________________
MBCC1pi0Model::~MBCC1pi0Model()
{

}
//____________________________________________________________________________
void MBCC1pi0Model::Write(const char* output_file)
{
  // Write arrays to root histograms

  /*
  // 
  TH1D * histo_genie[kNCosThetamuBins];
  for(int i = 0; i < kNCosThetamuBins; i++) {
    const char * title = "";
    const char * name_genie = Form("genie_costhetabin_%d",i);
    histo_genie[i] = new TH1D(name_genie, title, kNTmuBins, fTmuBinEdges);
  }

  // Fill root histogram with the data  
  for(int i = 0; i < kNCosThetamuBins; i++) {
    for(int j = 0; j < kNTmuBins; j++) {
      double xsec_genie = kdDiffXSecArrayGenie[i][j];
      histo_genie[i]->SetBinContent(j+1,xsec_genie);
    }
  }

  // Use ROOT TFile Class to write data, update mode, will be writing to existing file
  TFile f(output_file,"update");
  for(int i = 0; i < kNCosThetamuBins; i++) {
    histo_genie[i]->Write();
  }
  f.Close();
  */
}
//____________________________________________________________________________
bool MBCC1pi0Model::Generate(const char* event_file)
{
   // open the ROOT file and get the TTree & its header
  TTree *           tree = 0;
  NtpMCTreeHeader * thdr = 0;

  TFile file(event_file,"READ");

  tree = dynamic_cast <TTree *>           ( file.Get("gtree")  );
  thdr = dynamic_cast <NtpMCTreeHeader *> ( file.Get("header") );

  if(!tree) return false;

  NtpMCEventRecord * mcrec = 0;
  tree->SetBranchAddress("gmcrec", &mcrec);

/*
  int nev = (gOptNEvt > 0) ?
        TMath::Min(gOptNEvt, (int)tree->GetEntries()) :
        (int) tree->GetEntries();
*/

  int nev = (int) tree->GetEntries();

  //
  // Loop over all events
  //
  for(int k = 0; k < nev; k++) {

    // get next tree entry
    tree->GetEntry(k);

    // get the GENIE event
    EventRecord &  event = *(mcrec->event);

    const Interaction * interaction = event.Summary();

    int  nupdg     = event.Probe()->Pdg();
    bool isnumu    = (nupdg == kPdgNuMu);
    bool iscc      = interaction->ProcInfo().IsWeakCC();
    
    // Outputs events to screen -- 
    // LOG("myAnalysis", pNOTICE) << event;

    //
    // Loop over all particles in this event
    //
   
    bool   has_muon = false;  
    bool   has_pion = false;  
    double T_mu     = -999;
    double costh_mu = -999;

    int NfPip      = 0;
    int NfPim      = 0;
    int NfPi0      = 0;

    GHepParticle * p = 0;
    TIter event_iter(&event);

    while((p=dynamic_cast<GHepParticle *>(event_iter.Next())))
    {  
       if (p->Status() == kIStStableFinalState ) {

	  if (p->Pdg() == kPdgPiP) NfPip++;
	  if (p->Pdg() == kPdgPiM) NfPim++;
	  if (p->Pdg() == kPdgPi0) NfPi0++;
	  
	    // has_muon = true;
	    // T_mu = p->KinE();
	    // costh_mu = p->P4()->Pz() / p->P4()->P();
       }
    }// end loop over particles	in event
    
    // Check for single pi 0 in the final state
    bool is1pi0X  = (NfPip==0 && NfPi0==1 && NfPim==0);

    // event is CC 1 pi+ like if it has  
    bool kTpNumuCC1pi0 = (isnumu && iscc && is1pi0X);

    bool is_cc1pi0like = kTpNumuCC1pi0  ; // Cuts

    //bool is_qelike = has_muon && (T_mu > MB::T_mu_cut) && !has_pion;

    // bool is_qelike = has_muon && (T_mu > 0.2) && !has_pion;

    if(!is_cc1pi0like) continue;

    // put all CosThetamu below epsilon value in first bin 
    double epsilon = 1E-6;
    if(TMath::Abs(costh_mu-1) < epsilon) {
       costh_mu = costh_mu - epsilon;
    }
    // find correct bin (data file structure) for GENIE prediction   
    // int i = TMath::BinarySearch(1+CCQE::kNCosThetamuBins, fCosThetamuBinEdges, costh_mu);
    // int j = TMath::BinarySearch(1+CCQE::kNTmuBins,  fTmuBinEdges,      T_mu);

   
    // std::cout << "Bin Search i, j: " << i << ", " << j ;
    
    // Hook for future reweighting function 
    int weight = this->ReWeighting(event);

    // Add weighted event to bin 
    // kdDiffXSecArrayGenie[i][j] = weight + kdDiffXSecArrayGenie[i][j];
    std::cout << " added" << std::endl;
    // clear current mc event record
    mcrec->Clear();

  }//end loop over events

  // close input GHEP event file
  file.Close();

  LOG("myAnalysis", pNOTICE)  << "Done!";

  return true;
}
//____________________________________________________________________________
void MBCC1pi0Model::Print()
{

}
//____________________________________________________________________________
void MBCC1pi0Model::Initialise(double * binedges_data_array, int data_dims)
{
  for (int i = 0; i < data_dims; i++) {
    binedges_data_array[i] = 0;
  }
}
//____________________________________________________________________________
