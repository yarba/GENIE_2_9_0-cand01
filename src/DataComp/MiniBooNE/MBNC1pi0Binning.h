//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBNC1pi0Binning

\brief    Class to hold the binning information from the MiniBooNE Muon Neutrino 
          and Muon Antineutrino Single Pi0 production cross sections
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jan 26, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_READ_XSEC_SPI_BINS_H_
#define _MINIBOONE_READ_XSEC_SPI_BINS_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"

using std::string;

using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

class MBNC1pi0Binning
{ 

public:
  MBNC1pi0Binning();
  ~MBNC1pi0Binning();
  
  // Read all binning files to arrays 
  bool Read();

  // Print all arrays to screen 
  bool Print();
  
  // Allow access to private arrays containing bin information
  // Neutrino mode bin arrays

  const double* GetNuModeNuPpi0BinEdges()             const { return &fNuModeNuPpi0BinEdges[0]; }            
  const double*	GetNuModeNuCosThetapi0BinEdges()      const { return &fNuModeNuCosThetapi0BinEdges[0]; }     
  const double*	GetNuModeNuNuBarPpi0BinEdges()        const { return &fNuModeNuNuBarPpi0BinEdges[0]; }       
  const double*	GetNuModeNuNuBarCosThetapi0BinEdges() const { return &fNuModeNuNuBarCosThetapi0BinEdges[0]; }

  // Antineutrino mode bin arrays

  const double*  GetNuBarModePpi0BinEdges()               const { return &fNuBarModePpi0BinEdges[0]; }       
  const double*	 GetNuBarModeCosThetapi0BinEdges()        const { return &fNuBarModeCosThetapi0BinEdges[0]; }
  const double*	 GetNuBarModeNuNuBarPpi0BinEdges()        const { return &fNuBarModePpi0BinEdges[0]; }       
  const double*	 GetNuBarModeNuNuBarCosThetapi0BinEdges() const { return &fNuBarModeCosThetapi0BinEdges[0]; }

private:
  
  // Read binning data into corresponding array
  bool ReadtoArray(std::string data_bins_file, double* data_array);
  bool PrintArray(double* data_array, int array_size);
  bool InitialiseArray(double* data_array, int array_size);
  
  // Neutrino mode binning arrays
  double fNuModeNuPpi0BinEdges             [NC1pi0::kNNuModeNuPpi0Bins             + 1];
  double fNuModeNuCosThetapi0BinEdges      [NC1pi0::kNNuModeNuCosThetapi0Bins      + 1];
  double fNuModeNuNuBarPpi0BinEdges        [NC1pi0::kNNuModeNuNuBarPpi0Bins        + 1]; 
  double fNuModeNuNuBarCosThetapi0BinEdges [NC1pi0::kNNuModeNuNuBarCosThetapi0Bins + 1]; 
  
  // Antineutrino mode binning arrays
  double fNuBarModePpi0BinEdges               [NC1pi0::kNNuBarModeNuPpi0Bins             + 1];
  double fNuBarModeCosThetapi0BinEdges        [NC1pi0::kNNuBarModeNuCosThetapi0Bins      + 1];  
  double fNuBarModeNuNuBarPpi0BinEdges        [NC1pi0::kNNuBarModeNuNuBarPpi0Bins        + 1]; 
  double fNuBarModeNuNubarCosThetapi0BinEdges [NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins + 1]; 
 
  // Path to data in Genie
  std::string fNC1pi0Path;

  // Data file variables
  std::string fnu_mode_nu_1pi0_Ppi0   	            ;
  std::string fnu_mode_nu_1pi0_CosThetapi0          ;
  std::string fnu_mode_nu_nubar_1pi0_Ppi0           ;
  std::string fnu_mode_nu_nubar_1pi0_CosThetapi0    ;

  std::string fnubar_mode_nu_1pi0_Ppi0 	            ;
  std::string fnubar_mode_nu_1pi0_CosThetapi0       ;
  std::string fnubar_mode_nu_nubar_1pi0_Ppi0        ;
  std::string fnubar_mode_nu_nubar_1pi0_CosThetapi0 ;
  
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_READ_XSEC_SPI_BINS_H_
