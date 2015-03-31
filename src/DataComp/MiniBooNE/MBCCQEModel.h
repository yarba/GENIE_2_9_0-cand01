//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCCQEModel

\brief    GENIE predictions of the MiniBooNE CCQE double differential 
          neutrino cross-sections
          
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

#ifndef _MINIBOONE_CCQE_XSEC_MODEL_H_
#define _MINIBOONE_CCQE_XSEC_MODEL_H_

#include "Utils/GArray.h"

#include "DataComp/Base/GExModelABC.h"
#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBCCQEBinning.h"
#include "DataComp/MiniBooNE/MBCCQEData.h"

class TChain;

namespace genie {
class GSimFiles;
class NtpMCEventRecord;
class EventRecord;
}

using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

class MBCCQEModel: public GExModelABC
{
public:
  MBCCQEModel();
  virtual ~MBCCQEModel();
  
  // Implement the GExModelABC interface
  // See GExModelABC.h for detailed comments on the purpose of each method.
  string          Name     (void);
  const Registry& Metadata (void) const;
  bool            Read     (string file, Option_t * opt = 0);
  virtual bool    Generate (Option_t * opt = 0);
  bool            Write    (TObjArray* collection, const char * tag = "genie", Option_t * write_opt = 0);
  bool            Write    (const char * filename, const char * tag = "genie", Option_t * write_opt = 0, Option_t * file_opt = 0);  
  virtual double  Weight   (EventRecord & ) { return 1.; }

protected:

  void Init  (void);  // Initialize data members
  void Reset (void);  // Reset predictions
  void Print (void);  // Formatted output to screen  
  void Norm  (void);  // Normalization calculation

  //
  // Data members
  //

  // Info read from the XML file 
  MBDataRelease_t    fDataRelease;        // Name of MB data release GENIE is compared against
  string             fBinningFilename;    // File holding binning info
  string             fGFileListFilename;  // XML file (GSimFiles schema) with GENIE cross-section and event files
  string             fGModelName;         // GENIE model name (to pick right subset of input event files)
  string             fFluxFilename;       // MiniBooNE flux file (for absolute normalization calculation)
  string             fFluxHistname;       // Name of histogram in MiniBooNE flux file
  double             fMass;               // fiducial mass in tonnes (for absolute normalization calculation)
  double             fExposureData;       // exposure in POT for corresponding dataset (for absolute normalization calculation)

  //  
  Registry           fMetadata;           //
  TH1D *             fFlux;               // MiniBooNE flux histogram (for absolute normalization calculation)
  GSimFiles *        fGFileList;          // GENIE cross-section and event files 
  TChain *           fEventChain;         // Selected chain of files based on model name
  NtpMCEventRecord * fMCRecord;           //
  MBCCQEBinning *    fBinning;            // Bin edge arrays. They depend on the CCQE data release 
  GArray2D *         fGdDiffXSec;         // GENIE calculated double-differential cross-section central values, TNamed derived array for persistency  
  double             fExposureMC;         // exposure in POT for input MC sample (for absolute normalization calculation)
  double             fNormScale;          // MC normalization scale
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_CCQE_XSEC_MODEL_H_
