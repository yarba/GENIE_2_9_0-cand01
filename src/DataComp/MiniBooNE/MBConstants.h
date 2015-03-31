//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::constants::MB

\brief    Class to hold the MiniBooNE data constant values          
          
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


#ifndef _MINIBOONE_CONSTANTS_H_
#define _MINIBOONE_CONSTANTS_H_

#include <string>

namespace genie {
namespace mc_vs_data {
namespace constants {
namespace MB {
  
namespace CCQE {

  // Charged Current Quasielastic Data (MBCCQE*)
  // miniboone 
  // Number of bins for each kinematic variable

  static const int kNCosThetamuBins = 20;
  static const int kNTmuBins        = 18;

  // Experimental cuts
  static const double T_mu_cut = 0.2;  
    
  // Normalisation Uncertainty
  static const double kNuNormUncert    = 0.107; // 10.7% Nu
  static const double kNuBarNormUncert = 0.130; // 13.0% Nubar
  
  static const double kNuUncertScaling = 0.1; // Units of error are 1e-42 data is 1e-41
  
} // CCQE namespace

namespace 
CC1pip {
  
  // Charged Current Single Pi+ Data (MBCC1pip*)
  // data_release/ccpip/

  static const int kNEnuBins = 27;
  static const int kNTpiBins = 15;
  static const int kNQ2Bins  = 23;
  static const int kNTmuBins = 23;

  // Upper bin edge values not included in the text file but can be read from .root file
  static const int kNEnuLastBin = 2.0e3;
  static const int kNTpiLastBin = 4.0e2;
  static const int kNQ2LastBin  = 1.5e6;
  static const int kNTmuLastBin = 1.5e3;


  static const int kNDiffQ2EnuQ2Bins   = 22;
  static const int kNDiffQ2EnuBins     = 23;
  static const int kNDiffTmuEnuTmuBins = 23;
  static const int kNDiffTmuEnuBins    = 23;
  static const int kNDiffTpiEnuTpiBins = 15;
  static const int kNDiffTpiEnuBins    = 23;

  // Upper bin edge values
  static const int kNDiffQ2EnuQ2LastBin   = 1.3e6; 
  static const int kNDiffQ2EnuLastBin     = 1.7e3; 
  static const int kNDiffTmuEnuTmuLastBin = 1.5e3;
  static const int kNDiffTmuEnuLastBin    = 1.7e3;
  static const int kNDiffTpiEnuTpiLastBin = 4.0e2;
  static const int kNDiffTpiEnuLastBin    = 1.7e3;


  static const int kNDbldiffCosThetamuBins = 29;
  static const int kNDbldiffTmuBins        = 20;
  static const int kNDbldiffCosThetapiBins = 25;
  static const int kNDbldiffTpiBins        = 10;

  // Upper bin edge value
  static const int kNDbldiffCosThetamuLastBin = 1;
  static const int kNDbldiffTmuLastBin        = 1.5e3;
  static const int kNDbldiffCosThetapiLastBin = 1;
  static const int kNDbldiffTpiLastBin        = 4.0e2;

  // Data cuts
  static const int pi_sum_nucl_cut = 1.35 ; // pion + N mass < 1350 MeV/c^2

} //namespace CC1pip

namespace CC1pi0 {
  
  // Charged Current Single Pi0 Data (MBCC1pi0*)
  // data_release/ccpi0/
  
  static const int kNEnuBins         = 14;
  static const int kNQ2Bins          = 12;
  static const int kNEmuBins         = 13;
  static const int kNCosThetamuBins  = 9;
  static const int kNPpi0Bins        = 11;
  static const int kNCosThetapi0Bins = 15;

} // CC1pi0 namespace

namespace NC1pi0 {
  
  // Neutral Current Single Pi0 Data (MBNC1pi0*)
  // data_release/ncpi0/

  // Data Path
  static const std::string pi0Path;
  
  // Neutrino Mode
  static const int kNNuModeNuPpi0Bins             = 11; 
  static const int kNNuModeNuCosThetapi0Bins      = 18;
  static const int kNNuModeNuNuBarPpi0Bins        = 11; 
  static const int kNNuModeNuNuBarCosThetapi0Bins = 18; 
  
  // Antineutrino Mode
  static const int kNNuBarModeNuPpi0Bins             = 10; 
  static const int kNNuBarModeNuCosThetapi0Bins      = 10;
  static const int kNNuBarModeNuNuBarPpi0Bins        = 10; 
  static const int kNNuBarModeNuNuBarCosThetapi0Bins = 10; 

} // NC1pi0 namespace
   
} // MB namespace
} // constants namespace
} // mc_vs_data namespace
} // genie namepace

 
#endif // _MINIBOONE_CONSTANTS_H_
