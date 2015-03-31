//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBCCQEData

\brief    MiniBooNE CCQE double differential neutrino cross-section data.

\refs     > 2010 Neutrino CCQE data release
            http://www-boone.fnal.gov/for_physicists/data_release/ccqe/
            Phys. Rev. D81, 092005 (2010) (see p. 15 fig. 13)
          > 2013 Anti-neutrino CCQE data release
            http://www-boone.fnal.gov/for_physicists/data_release/ccqe_nubar
            Phys. Rev. D88, 032001 (2013) (see p. 12 figs. 8 & 9)
          
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

#ifndef _MINIBOONE_CCQE_XSEC_DATA_H_
#define _MINIBOONE_CCQE_XSEC_DATA_H_

#include <string>

#include "Registry/Registry.h"
#include "DataComp/Base/GExDataABC.h"
#include "DataComp/Base/GExModelABC.h"
#include "DataComp/MiniBooNE/MBDataRelease.h"
#include "DataComp/MiniBooNE/MBConstants.h"

namespace genie {
 class GArray2D;
 class GExModelABC;
 namespace mc_vs_data {
   class MBCCQEBinning;
 }
}

using std::string;
using namespace genie::mc_vs_data::constants::MB;

namespace genie {
namespace mc_vs_data {

class MBCCQEData: public GExDataABC
{ 
public:
  MBCCQEData();
 ~MBCCQEData();

  // Implement the GExDataABC interface
  // See GExDataABC.h for detailed comments on the purpose of each method.
  string           Name      (void);
  const Registry & Metadata  (void) const;
  bool             Read      (string file, Option_t * opt = 0);
  bool             Generate  (GExModelABC * model, bool fluctuate=true);
  bool             Write     (TObjArray * collection, const char * tag = "data", Option_t * write_opt = 0);
  bool             Write     (const char * filename,  const char * tag = "data", Option_t * write_opt = 0, Option_t * file_opt = 0);
  int              NumOfBins (void);

private:

  void Init  (void);             // Initialize
  void Print (void);             // Print double differential data to screen
  void Print (GArray2D * array);

  bool ReadMBFile (string file_name, GArray2D * array);  // Read data from official MiniBooNE data-files

  //
  // Data members
  //

  // Info read from the XML file
  MBDataRelease_t    fDataRelease;            // Name MB data release
  string             fBinningFilename;        // File with binning info
  string             fXSecDataFilename;       // File with double-differential cross-section central values
  string             fXSecShapeErrorFilename; // File with double-differential cross-section shape err

  // Objects where info read from input XML file ends up to
  Registry        fMetadata;           //	
  MBCCQEBinning * fBinning;            // Bin edge arrays. They depend on the CCQE data release
  GArray2D *      fdDiffXSecCentrVal;  // Double-differential cross-section central values, TNamed derived array for persistency
  GArray2D *      fdDiffXSecShapeErr;  // Double-differential cross-section shape err, TNamed derived array for persistency
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_CCQE_XSEC_DATA_H_
