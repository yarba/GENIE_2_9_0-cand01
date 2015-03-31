//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCC1pipModel

\brief    Class to hold the Genie Predicted MiniBooNE CC 1 pi+ 
          differential neutrino cross-section data.
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool
	  
	  Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          University of Liverpool & STFC Rutherford Appleton Laboratory    

\created  Jan 11, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________


#ifndef _MINIBOONE_CC1PIP_PREDICTION_H_
#define _MINIBOONE_CC1PIP_PREDICTION_H_

#include "EVGCore/EventRecord.h"
#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBCC1pipBinning.h"

using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

class MBCC1pipModel
{
public:

  MBCC1pipModel(MBCC1pipBinning & binning);
  virtual ~MBCC1pipModel();
  
  // Write Results of Generate to ROOT file
  //  void Write(const char* output_file);
  void Write(TObjArray * array, const char * tag, Option_t * write_opt);

  
  // Loop over events in input file and generate prediction
  bool Generate(const char* event_file);

  // Formatted output to screen
  void Print();
  
protected:

  // Function hook to allow for reweighting 
  virtual int ReWeighting(EventRecord & weight) { return 1; }
  void Initialise(double * array, int array_dim);
  
  // Arrays for GENIE prediction

  //
  // Wrap in GArray1D or GArray2D -- Add Init method (is this done already), 
  //
  
  // Integrated XSec
  double fEnuXSecGenie  [CC1pip::kNEnuBins];

  //Single differential XSec 
  double fTpiXSecGenie  [CC1pip::kNTpiBins];
  double fQ2XSecGenie   [CC1pip::kNQ2Bins ];
  double fTmuXSecGenie  [CC1pip::kNTmuBins];
 
  // Differential XSecGenie data arrays
  double fDiffQ2EnuXSecGenie  [CC1pip::kNDiffQ2EnuBins     * CC1pip::kNDiffQ2EnuQ2Bins];
  double fDiffTmuEnuXSecGenie [CC1pip::kNDiffTmuEnuTmuBins * CC1pip::kNDiffTmuEnuBins ];
  double fDiffTpiEnuXSecGenie [CC1pip::kNDiffTpiEnuTpiBins * CC1pip::kNDiffTpiEnuBins ];

  // Double differential XSecGenie data arrays
  double fDbldiffTmuCosmuXSecGenie [CC1pip::kNDbldiffCosThetamuBins * CC1pip::kNDbldiffTmuBins];
  double fDbldiffTpiCospiXSecGenie [CC1pip::kNDbldiffCosThetapiBins * CC1pip::kNDbldiffTpiBins];

 
  // Arrays from MBCC1pipBinning object set in constructor
  //
  // Change this so that a CC1pip Binning object is passed instead
  //
  const double * fEnuBinEdges ;              
  const double * fTpiBinEdges ;             
  const double * fQ2BinEdges  ;             
  const double * fTmuBinEdges ;             
         
  const double * fDiffQ2EnuQ2BinEdges   ;     
  const double * fDiffQ2EnuBinEdges     ;
  const double * fDiffTmuEnuTmuBinEdges ;
  const double * fDiffTmuEnuBinEdges    ;
  const double * fDiffTpiEnuTpiBinEdges ;
  const double * fDiffTpiEnuBinEdges    ;
  
  const double * fDbldiffCosThetamuBinEdges ;
  const double * fDbldiffTmuBinEdges        ;
  const double * fDbldiffCosThetapiBinEdges ;
  const double * fDbldiffTpiBinEdges        ;



};

} // mc_vs_data namespace
} // genie namepace


#endif // _MINIBOONE_CC1PIP_PREDICTION_H_
