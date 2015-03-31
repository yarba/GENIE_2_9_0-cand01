//____________________________________________________________________________
/*!

\class    genie::tune::MBCCQEFitModel

\brief    MiniBooNE CCQE fit model.
          A minimal class that interfaces MBCCQEModel and the reweighting.
          
\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          University of Liverpool & STFC Rutherford Appleton Lab

\created  Jan 11, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _MINIBOONE_CCQE_XSEC_FIT_MODEL_H_
#define _MINIBOONE_CCQE_XSEC_FIT_MODEL_H_

#include "ReWeight/GReWeight.h"
#include "DataComp/MiniBooNE/MBCCQEModel.h"
#include "Tuning/GExFitModelABC.h"

using namespace genie::rew;
using namespace genie::mc_vs_data;

namespace genie {
namespace tune  {

class MBCCQEFitModel: public GExFitModelABC, public MBCCQEModel
{
public:
  MBCCQEFitModel();
  virtual ~MBCCQEFitModel();

  // Override MBCCQEModel methods
  string Name     (void);
  bool   Read     (string file, Option_t * opt = 0);
  bool   Generate (Option_t * opt = 0);
  bool   Write    (TObjArray* collection, const char * tag = "model", Option_t * write_opt = 0);
  bool   Write    (const char * filename, const char * tag = "model", Option_t * write_opt = 0, Option_t * file_opt = 0);
  double Weight   (EventRecord & event);

  // Implement GExFitModelABC methods
  bool   Generate  (const GFitParamList & fp, Option_t * opt = 0);
  double Statistic (GExDataABC * data, const GFitParamList & fp, Option_t * opt = 0);
  
private:

//  MBCCQEModel * fMBCCQEModel;

  double ReWeighting(EventRecord & event);  

  GReWeight * fRwght; // GENIE event reweighting interface

};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_CCQE_XSEC_FIT_MODEL_H_
