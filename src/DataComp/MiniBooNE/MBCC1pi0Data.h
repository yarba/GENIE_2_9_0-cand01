//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCC1pi0Data

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

#ifndef _MINIBOONE_NU_CC1PI0_XSEC_DATA_H_
#define _MINIBOONE_NU_CC1PI0_XSEC_DATA_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBCC1pi0Data.h"
#include "DataComp/MiniBooNE/MBCC1pi0Binning.h"

using std::string;
using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {


class MBCC1pi0Data
{ 

public:

  MBCC1pi0Data(MBCC1pi0Binning & binning);
  ~MBCC1pi0Data();

  // Read data files and error matrices into arrays
  bool Read();

  // Write root histogram with double differential data
  void Write(const char* output_file);

  // Print double differential data to screen
  void Print();

private:
  
  // Read binning data into corresponding array
  bool ReadArray(std::string BinEdgesFile, double * data_array, std::string read_string);
  bool PrintArray(double* data_array, int array_size);

  bool Initialise(double* data_array, int array_size);
  void WriteSingle(double * array, const double * edges_array, const int array_bound, std::string name_label, const char * output_file);



  // Binning array pointers
  // Neutrino mode binning arrays passed from MBCC1pi0 binning
  const double * fEnuBinEdges         ;
  const double * fQ2BinEdges          ;
  const double * fEmuBinEdges         ;
  const double * fCosThetamuBinEdges  ;
  const double * fPpi0BinEdges        ;
  const double * fCosThetapi0BinEdges ;

  
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

 
  // Path to data in Genie
  std::string fCC1pi0Path;

  // Cross section data file variables
  std::string fEnuData_file         ;
  std::string fQ2Data_file          ; 
  std::string fEmuData_file         ;  
  std::string fCosThetamuData_file  ;
  std::string fPpi0Data_file        ;
  std::string fCosThetapi0Data_file ;

};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_NU_CC1PI0_XSEC_DATA_H_
