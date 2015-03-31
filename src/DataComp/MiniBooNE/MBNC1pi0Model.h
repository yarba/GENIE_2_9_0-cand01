//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBNC1pi0Model

\brief    Class to hold the Genie Predicted MiniBooNE Single Pi0
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool
	  
	  Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          University of Liverpool & STFC Rutherford Appleton Laboratory    

\created  Jan 26, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________


#ifndef _MINIBOONE_NC1PI0_PREDICTION_H_
#define _MINIBOONE_NC1PI0_PREDICTION_H_

#include "EVGCore/EventRecord.h"
#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBNC1pi0Binning.h"

using namespace genie::mc_vs_data::constants::MB::NC1pi0;

namespace genie {
namespace mc_vs_data {

class MBNC1pi0Model
{
public:

  MBNC1pi0Model(MBNC1pi0Binning & binning);
  virtual ~MBNC1pi0Model();
  
  // Write Results of Generate to ROOT file
  void Write(const char* output_file);
  
  // Loop over events in input file and generate prediction
  bool Generate(const char* event_file);

  // Formatted output to screen of arrays
  void Print();
  
protected:

  // Function hook to allow for reweighting 
  virtual int ReWeighting(EventRecord & weight) { return 1; }

  void Initialise(double * array, int array_dim);
  
  // Bin numbers for double differential data
  // static const int kNCosThetamuBins = constants::MB::kNCosThetamuBins ;
  //static const int kNTmuBins        = constants::MB::kNTmuBins;
 
  // Double differential array for GENIE prediction
  //double kdDiffXSecArrayGenie[kNCosThetamuBins][kNTmuBins];

  // Arrays from MBNC1pi0Binning object set in constructor
  //const double * fCosThetamuBinEdges;
  //const double * fTmuBinEdges;    
};

} // mc_vs_data namespace
} // genie namepace


#endif // _MINIBOONE_NC1PI0_PREDICTION_H_
