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
#include "DataComp/MiniBooNE/MBCC1pipModel.h"
#include "DataComp/MiniBooNE/MBConstants.h"

using namespace genie;
using namespace genie::mc_vs_data;
//using namespace genie::mc_vs_data::constants;
using namespace genie::mc_vs_data::constants::MB;

//____________________________________________________________________________
MBCC1pipModel::MBCC1pipModel(MBCC1pipBinning & binning) 
{

  // Initialise Xsec arrays
  Initialise( fEnuXSecGenie , CC1pip::kNEnuBins  );
  Initialise( fTpiXSecGenie , CC1pip::kNTpiBins  );
  Initialise( fQ2XSecGenie  , CC1pip::kNQ2Bins   );
  Initialise( fTmuXSecGenie , CC1pip::kNTmuBins  );

  Initialise( fDiffQ2EnuXSecGenie       , ( CC1pip::kNDiffQ2EnuBins     * CC1pip::kNDiffQ2EnuQ2Bins    )  );
  Initialise( fDiffTmuEnuXSecGenie      , ( CC1pip::kNDiffTmuEnuTmuBins * CC1pip::kNDiffTmuEnuBins     )  );
  Initialise( fDiffTpiEnuXSecGenie      , ( CC1pip::kNDiffTpiEnuTpiBins * CC1pip::kNDiffTpiEnuBins     )  ); 
  Initialise( fDbldiffTmuCosmuXSecGenie , ( CC1pip::kNDbldiffCosThetamuBins * CC1pip::kNDbldiffTmuBins )  );
  Initialise( fDbldiffTpiCospiXSecGenie , ( CC1pip::kNDbldiffCosThetapiBins * CC1pip::kNDbldiffTpiBins )  );

  // Get pointers to binning arrays
  fEnuBinEdges = binning.GetEnuBinEdges();              
  fTpiBinEdges = binning.GetTpiBinEdges();              
  fQ2BinEdges  = binning.GetQ2BinEdges() ;              
  fTmuBinEdges = binning.GetTmuBinEdges();              
  					                                 
  fDiffQ2EnuQ2BinEdges   = binning.GetDiffQ2EnuQ2BinEdges()   ;   
  fDiffQ2EnuBinEdges     = binning.GetDiffQ2EnuBinEdges()     ;
  fDiffTmuEnuTmuBinEdges = binning.GetDiffTmuEnuTmuBinEdges() ;
  fDiffTmuEnuBinEdges    = binning.GetDiffTmuEnuBinEdges()    ;
  fDiffTpiEnuTpiBinEdges = binning.GetDiffTpiEnuTpiBinEdges() ;
  fDiffTpiEnuBinEdges    = binning.GetDiffTpiEnuBinEdges()    ;
  			     	                                      
  fDbldiffCosThetamuBinEdges = binning.GetDbldiffCosThetamuBinEdges() ;
  fDbldiffTmuBinEdges        = binning.GetDbldiffTmuBinEdges()        ;
  fDbldiffCosThetapiBinEdges = binning.GetDbldiffCosThetapiBinEdges() ;
  fDbldiffTpiBinEdges        = binning.GetDbldiffTpiBinEdges()        ;
 
}
//____________________________________________________________________________
MBCC1pipModel::~MBCC1pipModel()
{

}
//____________________________________________________________________________
void MBCC1pipModel::Write(TObjArray * array, const char * tag, Option_t * write_opt)
{
 
}
//____________________________________________________________________________
bool MBCC1pipModel::Generate(const char* event_file)
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
    NtpMCRecHeader rec_header = mcrec->hdr;
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

    int NfPip = 0;
    int NfPim = 0;
    int NfPi0 = 0;

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
    

    // Check for single pi + in the final state
    bool is1pipX  = (NfPip==1 && NfPi0==0 && NfPim==0);
    
    // event is CC 1 pi+ like if weak CC interaction and 
    // bool kTpNumuCC1pip = (isnumu && iscc && is1pipX);

    // Get mass of nucleon and pion and sum
    double mass_sum_pi_nucleon = 0;
    // 
    // bool is_cc1piplike = kTpNumuCC1pip && ( mass_sum_pi_nucleon  < CC1pip::pi_sum_nucl_cut) ; // Cuts

    if(!is1pipX) 
      {
	//LOG("is1pipX", pNOTICE) << is1pipX;
	continue;
      }
    // if(is_cc1piplike) continue;

    LOG("myPionXsec", pNOTICE) << "is 1pipX: " << is1pipX
      // << " kTpNumuCC1pip: " << kTpNumuCC1pip
			       << " Cross section: " << event.XSec()  
			       << " Energy: " << event.Probe()->E();  
    // LOG("myPionXsec", pNOTICE) << "\n"
    // 			       << " NfPip: " << NfPip
    // 			       << "**Event: " << rec_header.ievent 
    // 			       << " Cross section: " << event.XSec() 
    // 			       << " Energy: " << event.Probe()->E();  
    
    // put all CosThetamu below epsilon value in first bin 
    double epsilon = 1E-6;
    if(TMath::Abs(costh_mu-1) < epsilon) {
       costh_mu = costh_mu - epsilon;
    }

    // Hook for future reweighting function 
    int weight = this->ReWeighting(event);

    // Add weighted event to bin 
    //kdDiffXSecArrayGenie[i][j] = weight + kdDiffXSecArrayGenie[i][j];
    
    // clear current mc event record
    mcrec->Clear();

  }//end loop over events

  // close input GHEP event file
  file.Close();

  LOG("myAnalysis", pNOTICE)  << "Done!";

  return true;
}
//____________________________________________________________________________
void MBCC1pipModel::Print()
{
 

}
//____________________________________________________________________________
void MBCC1pipModel::Initialise(double * binedges_data_array, int data_dims)
{
  for (int i = 0; i < data_dims; i++) {
    binedges_data_array[i] = 0;
  }
}
