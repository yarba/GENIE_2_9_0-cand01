//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBNC1pi0Data

\brief    Class to hold the MiniBooNE Single NC1pi0
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jan 26, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_NU_SPI_XSEC_DATA_H_
#define _MINIBOONE_NU_SPI_XSEC_DATA_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBNC1pi0Data.h"
#include "DataComp/MiniBooNE/MBNC1pi0Binning.h"

using std::string;

namespace genie {
namespace mc_vs_data {

using namespace constants::MB;

class MBNC1pi0Data
{ 

public:

  MBNC1pi0Data(MBNC1pi0Binning & binning);
 ~MBNC1pi0Data();

  // Read data files and error matrices into arrays
  bool Read();

  // Write root histogram with double differential data
  void Write(const char* output_file);

  // Print double differential data to screen
  void Print();

private:

  // Read binning data into corresponding array
  bool ReadtoArray(std::string data_bins_file, double * data_array);
  // Output array to screen
  bool PrintArray(double* data_array, int array_size);
  // Print an array with "2 dimensions"
  bool Print2DArray(double* data_array, int array_size_x, int array_size_y);
  // Read uncertainty
  bool ReadErrors(std::string data_error_file, double* data_error_array);
  bool InitialiseArray(double* data_array, int array_size);
  
  void ReplaceStringInPlace(string& subject, const string& search, const string& replace);

  // Binning array pointers
  // Neutrino mode binning arrays passed from MBNC1pi0 binning
  const double * fNuModeNuPpi0BinEdges             ;
  const double * fNuModeNuCosThetapi0BinEdges      ;
  const double * fNuModeNuNuBarPpi0BinEdges        ; 
  const double * fNuModeNuNuBarCosThetapi0BinEdges ; 
  
  // Antineutrino mode binning arrays
  const double * fNuBarModePpi0BinEdges               ;
  const double * fNuBarModeCosThetapi0BinEdges        ;  
  const double * fNuBarModeNuNuBarPpi0BinEdges        ; 
  const double * fNuBarModeNuNubarCosThetapi0BinEdges ; 
  
  // Xsec data arrays
  // Neutrino mode 
  double fNuModeNuPpi0Xsec             [NC1pi0::kNNuModeNuPpi0Bins            ];
  double fNuModeNuCosThetapi0Xsec      [NC1pi0::kNNuModeNuCosThetapi0Bins     ];
  double fNuModeNuNuBarPpi0Xsec        [NC1pi0::kNNuModeNuNuBarPpi0Bins       ]; 
  double fNuModeNuNuBarCosThetapi0Xsec [NC1pi0::kNNuModeNuNuBarCosThetapi0Bins]; 
	                                 
  // Antineutrino mode			 
  double fNuBarModePpi0Xsec               [NC1pi0::kNNuBarModeNuPpi0Bins            ];
  double fNuBarModeCosThetapi0Xsec        [NC1pi0::kNNuBarModeNuCosThetapi0Bins     ];  
  double fNuBarModeNuNuBarPpi0Xsec        [NC1pi0::kNNuBarModeNuNuBarPpi0Bins       ]; 
  double fNuBarModeNuNubarCosThetapi0Xsec [NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins]; 

  // Covariance Matrix Arrays
  // Neutrino mode
  double fNuModeNuPpi0Cov             [NC1pi0::kNNuModeNuPpi0Bins             * NC1pi0::kNNuModeNuPpi0Bins            ];
  double fNuModeNuCosThetapi0Cov      [NC1pi0::kNNuModeNuCosThetapi0Bins      * NC1pi0::kNNuModeNuCosThetapi0Bins     ];
  double fNuModeNuNuBarPpi0Cov        [NC1pi0::kNNuModeNuNuBarPpi0Bins        * NC1pi0::kNNuModeNuNuBarPpi0Bins       ]; 
  double fNuModeNuNuBarCosThetapi0Cov [NC1pi0::kNNuModeNuNuBarCosThetapi0Bins * NC1pi0::kNNuModeNuNuBarCosThetapi0Bins]; 

  // Antineutrino mode
  double fNuBarModePpi0Cov               [NC1pi0::kNNuBarModeNuPpi0Bins             * NC1pi0::kNNuBarModeNuPpi0Bins            ];
  double fNuBarModeCosThetapi0Cov        [NC1pi0::kNNuBarModeNuCosThetapi0Bins      * NC1pi0::kNNuBarModeNuCosThetapi0Bins     ];  
  double fNuBarModeNuNuBarPpi0Cov        [NC1pi0::kNNuBarModeNuNuBarPpi0Bins        * NC1pi0::kNNuBarModeNuNuBarPpi0Bins       ]; 
  double fNuBarModeNuNubarCosThetapi0Cov [NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins * NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins]; 
 
  // Path to data in Genie
  std::string fNC1pi0Path;

  // Cross section data file variables
  // Neutrino Mode
  std::string fnu_mode_nu_1pi0_Ppi0_xsec   	         ;
  std::string fnu_mode_nu_1pi0_CosThetapi0_xsec          ;
  std::string fnu_mode_nu_nubar_1pi0_Ppi0_xsec           ;
  std::string fnu_mode_nu_nubar_1pi0_CosThetapi0_xsec    ;
  // Antineutrino mode
  std::string fnubar_mode_nu_1pi0_Ppi0_xsec 	         ;
  std::string fnubar_mode_nu_1pi0_CosThetapi0_xsec       ;
  std::string fnubar_mode_nu_nubar_1pi0_Ppi0_xsec        ;
  std::string fnubar_mode_nu_nubar_1pi0_CosThetapi0_xsec ;

  // Covariance Matrix data files
  // Neutrino mode
  std::string fnu_mode_nu_1pi0_Ppi0_cov   	        ;
  std::string fnu_mode_nu_1pi0_CosThetapi0_cov          ;
  std::string fnu_mode_nu_nubar_1pi0_Ppi0_cov           ;
  std::string fnu_mode_nu_nubar_1pi0_CosThetapi0_cov    ;
  // Antineutrino mode
  std::string fnubar_mode_nu_1pi0_Ppi0_cov 	        ;
  std::string fnubar_mode_nu_1pi0_CosThetapi0_cov       ;
  std::string fnubar_mode_nu_nubar_1pi0_Ppi0_cov        ;
  std::string fnubar_mode_nu_nubar_1pi0_CosThetapi0_cov ;
 
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_NU_SPI_XSEC_DATA_H_
