//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCC1pipData

\brief    Class to hold the MiniBooNE Single NC1pi0
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool

\created  Feb 15, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_NU_CC1PIP_XSEC_DATA_H_
#define _MINIBOONE_NU_CC1PIP_XSEC_DATA_H_

#include <string>

#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBCC1pipData.h"
#include "DataComp/MiniBooNE/MBCC1pipBinning.h"

using std::string;
using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {


class MBCC1pipData
{ 

public:

  MBCC1pipData(MBCC1pipBinning & binning);
  ~MBCC1pipData();
  
  string Name     ();

  // Read data files and error matrices into arrays
  //bool Read();
  bool Read(string file, Option_t * opt);

  // Write root histogram with data
  //void Write();
  bool Write(TObjArray * array, const char * tag, Option_t * write_opt);


  // Print data to screen
  void Print();
 
 
  /*

  // Implement the GExDataABC interface
  // See GExDataABC.h for detailed comments on the purpose of each method.
  string Name      (void);
  bool   Read      (string file, Option_t * opt = 0);
  bool   Generate  (GExModelABC * model, const GFitParamList & fp, bool fluctuate=true);
  bool   Write     (TObjArray * collection, const char * tag = "data", Option_t * write_opt = 0);
  bool   Write     (const char * filename,  const char * tag = "data", Option_t * write_opt = 0, Option_t * file_opt = 0);
  int    NumOfBins (void);
  double Statistic (GExModelABC * pred, const GFitParamList & fp,   Option_t * stat_opt = "likelihood-ratio");
  double Statistic (GExModelABC * pred, Option_t * stat_opt = "likelihood-ratio");

   */

protected:
 
  bool PrintArray(double* data_array, int array_size);
  bool Initialise(double* data_array, int array_size);
  void ReplaceStringInPlace(std::string& subject, std::string& search, std::string& replace);
  void ReverseByRow(double * array, int width , int height);

  void WriteMtx(std::string fileLine, int & Nrow, int & Ncol, double * xyBins, int & i);
  bool ReadDataFile(string data_file);

  // Binning array pointers
  // Binning arrays passed from MBCC1pip binning
  const double * fEnuBinEdges ;              
  const double * fTpiBinEdges ;             
  const double * fQ2BinEdges  ;             
  const double * fTmuBinEdges ;             

  const double * fDiffQ2EnuQ2BinEdges      ;     
  const double * fDiffQ2EnuBinEdges        ;
  const double * fDiffTmuEnuTmuBinEdges    ;
  const double * fDiffTmuEnuBinEdges       ;
  const double * fDiffTpiEnuTpiBinEdges    ;
  const double * fDiffTpiEnuBinEdges       ;

  const double * fDbldiffCosThetamuBinEdge ;
  const double * fDbldiffTmuBinEdges       ;
  const double * fDbldiffCosThetapiBinEdge ;
  const double * fDbldiffTpiBinEdges       ;
  
  // Xsec data arrays
  double fEnuXSec  [CC1pip::kNEnuBins ];
  double fTpiXSec  [CC1pip::kNTpiBins ];
  double fQ2XSec   [CC1pip::kNQ2Bins  ];
  double fTmuXSec  [CC1pip::kNTmuBins ];
 
  // Differential XSec data arrays
  double fDiffQ2EnuXSec  [CC1pip::kNDiffQ2EnuBins     * CC1pip::kNDiffQ2EnuQ2Bins];
  double fDiffTmuEnuXSec [CC1pip::kNDiffTmuEnuTmuBins * CC1pip::kNDiffTmuEnuBins ];
  double fDiffTpiEnuXSec [CC1pip::kNDiffTpiEnuTpiBins * CC1pip::kNDiffTpiEnuBins ];

  // Double differential XSec data arrays
  double fDbldiffTmuCosmuXSec [CC1pip::kNDbldiffCosThetamuBins * CC1pip::kNDbldiffTmuBins];
  double fDbldiffTpiCospiXSec [CC1pip::kNDbldiffCosThetapiBins * CC1pip::kNDbldiffTpiBins];

  // Total uncertainty arrays
  double fEnuXSecUncert  [CC1pip::kNEnuBins ];
  double fTpiXSecUncert  [CC1pip::kNTpiBins ];
  double fQ2XSecUncert   [CC1pip::kNQ2Bins  ];
  double fTmuXSecUncert  [CC1pip::kNTmuBins ];
 
  // Percent Uncertainty Matrix Arrays
  double fDiffQ2EnuXSecUncert  [CC1pip::kNDiffQ2EnuBins     * CC1pip::kNDiffQ2EnuQ2Bins];
  double fDiffTmuEnuXSecUncert [CC1pip::kNDiffTmuEnuTmuBins * CC1pip::kNDiffTmuEnuBins ];
  double fDiffTpiEnuXSecUncert [CC1pip::kNDiffTpiEnuTpiBins * CC1pip::kNDiffTpiEnuBins ];

  double fDbldiffTmuCosmuXSecUncert [CC1pip::kNDbldiffCosThetamuBins * CC1pip::kNDbldiffTmuBins];
  double fDbldiffTpiCospiXSecUncert [CC1pip::kNDbldiffCosThetapiBins * CC1pip::kNDbldiffTpiBins];

   // Path to data in Genie
  std::string fCC1pipPath;

  // Cross section data file variables
  std::string fCC1pipData_file   ;
  
  // Binning info: Usually arrays holding bin edges
  MBCC1pipBinning * fBinning;
 
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_NU_CC1PIP_XSEC_DATA_H_
