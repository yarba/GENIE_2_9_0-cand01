//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCC1pi0Model

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


#ifndef _MINIBOONE_CC1PI0_PREDICTION_H_
#define _MINIBOONE_CC1PI0_PREDICTION_H_

#include "EVGCore/EventRecord.h"
#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBCC1pi0Binning.h"

using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

class MBCC1pi0Model
{
public:

  MBCC1pi0Model(MBCC1pi0Binning & binning);
  virtual ~MBCC1pi0Model();
  
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

  // Binning array pointers
  // Neutrino mode binning arrays passed from MBCC1pi0 binning
  const double * fEnuBinEdges         ;
  const double * fQ2BinEdges          ;
  const double * fEmuBinEdges         ;
  const double * fCosThetamuBinEdges  ;
  const double * fPpi0BinEdges        ;
  const double * fCosThetapi0BinEdges ;

  // Arrays for GENIE predictions
  
  // Xsec data arrays
  double fEnuXsec         [CC1pi0::kNEnuBins        ];
  double fQ2Xsec          [CC1pi0::kNQ2Bins         ];
  double fEmuXsec         [CC1pi0::kNEmuBins        ];
  double fCosThetamuXsec  [CC1pi0::kNCosThetamuBins ];
  double fPpi0Xsec        [CC1pi0::kNPpi0Bins       ];
  double fCosThetapi0Xsec [CC1pi0::kNCosThetapi0Bins];


  // Covariance Matrix Arrays
  double fEnuCov         [CC1pi0::kNEnuBins         * CC1pi0::kNEnuBins        ];
  double fQ2Cov          [CC1pi0::kNQ2Bins          * CC1pi0::kNQ2Bins         ];
  double fEmuCov         [CC1pi0::kNEmuBins         * CC1pi0::kNEmuBins        ];  
  double fCosThetamuCov  [CC1pi0::kNCosThetamuBins  * CC1pi0::kNCosThetamuBins ];
  double fPpi0Cov        [CC1pi0::kNPpi0Bins        * CC1pi0::kNPpi0Bins       ];
  double fCosThetapi0Cov [CC1pi0::kNCosThetapi0Bins * CC1pi0::kNCosThetapi0Bins];
 
};

} // mc_vs_data namespace
} // genie namepace


#endif // _MINIBOONE_CC1PI0_PREDICTION_H_
