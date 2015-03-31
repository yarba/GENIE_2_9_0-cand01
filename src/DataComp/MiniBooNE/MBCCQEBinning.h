//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCCQEBinning

\brief    Class to hold the binning information from the MiniBooNE QE double 
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Jan 11, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_CCQE_BINNING_H_
#define _MINIBOONE_CCQE_BINNING_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"
using std::string;

namespace genie {
namespace mc_vs_data {

using namespace constants::MB;
  
class MBCCQEBinning
{ 
public:
  MBCCQEBinning();
 ~MBCCQEBinning();
  
  // Read bin edges from data file
  bool Read(string binning_filename);
  
  // Allow access to private arrays containing bin information
  const double * GetCosThetamuBinEdges (void) const { return &fCosThetamuBinEdges[0]; }
  const double * GetTmuBinEdges        (void) const { return &fTmuBinEdges[0];        }
  
private:
 
  //
  // Hold data from 
  // http://www-boone.fnal.gov/for_physicists/data_release/ccqe/ 
  // in file aski_bin.txt --- Nu
  // http://www-boone.fnal.gov/for_physicists/data_release/ccqe_nubar/
  // in file dblDifflBins.txt --- Nubar

  string fBinningFilename; 
  double fCosThetamuBinEdges [CCQE::kNCosThetamuBins + 1];
  double fTmuBinEdges        [CCQE::kNTmuBins        + 1];  
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_READ_XSEC_BINS_H_
