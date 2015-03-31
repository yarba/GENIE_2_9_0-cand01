//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCC1piBinning

\brief    Class to hold the binning information from the MiniBooNE CC1pi0  
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Feb 6, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_READ_CC1PI0_XSEC_BINS_H_
#define _MINIBOONE_READ_CC1PI0_XSEC_BINS_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"

using std::string;
using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

  
class MBCC1pi0Binning
{ 

public:
  MBCC1pi0Binning();
  ~MBCC1pi0Binning();
  
  // Read binning file to array
  bool Read();
  
  // Allow access to private arrays containing bin information
  const double* GetEnuBinEdges()         const { return &fEnuBinEdges[0];         }
  const double* GetQ2BinEdges()          const { return &fQ2BinEdges[0];          }
  const double* GetEmuBinEdges()         const { return &fEmuBinEdges[0];         }
  const double* GetCosThetamuBinEdges()  const { return &fCosThetamuBinEdges[0];  }
  const double* GetPpi0BinEdges()        const { return &fPpi0BinEdges[0];        }
  const double* GetCosThetapi0BinEdges() const { return &fCosThetapi0BinEdges[0]; }
 
private:

  // Read in individual arrays
  bool ReadArray(std::string BinEdgesFile, double * data_array);
  bool Initialise(double * data_array, int data_dims);

  // Arrays of bin edges 
  double fEnuBinEdges         [CC1pi0::kNEnuBins         + 1];
  double fQ2BinEdges          [CC1pi0::kNQ2Bins          + 1];
  double fEmuBinEdges         [CC1pi0::kNEmuBins         + 1];
  double fCosThetamuBinEdges  [CC1pi0::kNCosThetamuBins  + 1];
  double fPpi0BinEdges        [CC1pi0::kNPpi0Bins        + 1];
  double fCosThetapi0BinEdges [CC1pi0::kNCosThetapi0Bins + 1];

  std::string fEnuBinEdges_file         ;
  std::string fQ2BinEdges_file          ; 
  std::string fEmuBinEdges_file         ;  
  std::string fCosThetamuBinEdges_file  ;
  std::string fPpi0BinEdges_file        ;
  std::string fCosThetapi0BinEdges_file ;

};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_READ_CC1PI0_XSEC_BINS_H_
