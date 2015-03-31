//____________________________________________________________________________
/*!

\class    genie::tune::GExFitModelABC

\brief    Pure abstract base class (pABC) - Augments the GExModelABC interface 
          with methods specific to model fitting/tuning
           
\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          University of Liverpool & STFC Rutherford Appleton Laboratory

          Adapted from VALOR oscillation fitting group code developed by
          C. Andreopoulos (Liverpool & STFC RAL), T. Dealtry (Oxford),
          S. Dennis (Warwick), L. Escudero (IFIC), N.Grant (Lancaster) and
          D.Sgalaberna (ETHZ).

\created  Feb 05, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _genie_expt_fit_model_abc_h_
#define _genie_expt_fit_model_abc_h_

#include <string>
#include <iostream>

//#include "DataComp/Base/GExModelABC.h"
#include "DataComp/Base/GExDataABC.h"
#include "Tuning/GFitParamList.h"

class TObjArray;

namespace genie {
namespace tune {

class GExFitModelABC  //: public GExModelABC
{
public:
  virtual ~GExFitModelABC();

  // Generate predictions for a given set of physics parameters
  virtual bool Generate (const GFitParamList & fp, Option_t * opt = 0) = 0;

  // Calculate \chisq, logL etc
  virtual double Statistic (GExDataABC * data, const GFitParamList & fp, Option_t * opt = 0) = 0;

protected:
   GExFitModelABC();
};

} // tune
} // genie

#endif

