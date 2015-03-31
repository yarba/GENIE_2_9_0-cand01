//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCC1piBinning

\brief    Class to hold the binning information from the MiniBooNE CC1pip  
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Feb 15, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_READ_CC1PIP_XSEC_BINS_H_
#define _MINIBOONE_READ_CC1PIP_XSEC_BINS_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"

using std::string;
using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

  
class MBCC1pipBinning
{ 

public:
  MBCC1pipBinning();
  ~MBCC1pipBinning();
  
  // Read binning file to array
  bool Read();
  
  // Allow access to private arrays containing bin information
  const double* GetEnuBinEdges()               const { return &fEnuBinEdges[0]; }
  const double* GetTpiBinEdges()               const { return &fTpiBinEdges[0]; }               
  const double*	GetQ2BinEdges()                const { return &fQ2BinEdges[0];  }                
  const double*	GetTmuBinEdges()               const { return &fTmuBinEdges[0]; }               
  					      
  const double*	GetDiffQ2EnuQ2BinEdges()       const { return &fDiffQ2EnuQ2BinEdges[0]; }       
  const double*	GetDiffQ2EnuBinEdges()         const { return &fDiffQ2EnuBinEdges[0];   }         
  const double*	GetDiffTmuEnuTmuBinEdges()     const { return &fDiffTmuEnuTmuBinEdges[0]; }     
  const double*	GetDiffTmuEnuBinEdges()        const { return &fDiffTmuEnuBinEdges[0];    }        
  const double*	GetDiffTpiEnuTpiBinEdges()     const { return &fDiffTpiEnuTpiBinEdges[0]; }     
  const double*	GetDiffTpiEnuBinEdges()        const { return &fDiffTpiEnuBinEdges[0];    }        
  					      
  const double*	GetDbldiffCosThetamuBinEdges() const { return &fDbldiffCosThetamuBinEdges[0]; } 
  const double*	GetDbldiffTmuBinEdges()        const { return &fDbldiffTmuBinEdges[0];        }    
  const double*	GetDbldiffCosThetapiBinEdges() const { return &fDbldiffCosThetapiBinEdges[0]; } 
  const double*	GetDbldiffTpiBinEdges()        const { return &fDbldiffTpiBinEdges[0];        }  


private:

  // Read in individual arrays
  bool ReadArray(std::string BinEdgesFile, double * data_array);
  bool Initialise(double * data_array, int data_dims, double last_bin_value);
  void ReplaceStringInPlace(std::string& subject, std::string& search, std::string& replace);

  // Arrays of bin edges 
  double fEnuBinEdges               [CC1pip::kNEnuBins + 1];
  double fTpiBinEdges               [CC1pip::kNTpiBins + 1];
  double fQ2BinEdges                [CC1pip::kNQ2Bins  + 1];
  double fTmuBinEdges               [CC1pip::kNTmuBins + 1];

  double fDiffQ2EnuQ2BinEdges       [CC1pip::kNDiffQ2EnuQ2Bins       + 1]; 
  double fDiffQ2EnuBinEdges         [CC1pip::kNDiffQ2EnuBins         + 1]; 
  double fDiffTmuEnuTmuBinEdges     [CC1pip::kNDiffTmuEnuTmuBins     + 1];
  double fDiffTmuEnuBinEdges        [CC1pip::kNDiffTmuEnuBins        + 1];
  double fDiffTpiEnuTpiBinEdges     [CC1pip::kNDiffTpiEnuTpiBins     + 1];
  double fDiffTpiEnuBinEdges        [CC1pip::kNDiffTpiEnuBins        + 1];
                                                     	  
  double fDbldiffCosThetamuBinEdges [CC1pip::kNDbldiffCosThetamuBins + 1];
  double fDbldiffTmuBinEdges        [CC1pip::kNDbldiffTmuBins        + 1];
  double fDbldiffCosThetapiBinEdges [CC1pip::kNDbldiffCosThetapiBins + 1];
  double fDbldiffTpiBinEdges        [CC1pip::kNDbldiffTpiBins        + 1]; 

  std::string fCC1pipData_file;

};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_READ_CC1PIP_XSEC_BINS_H_
