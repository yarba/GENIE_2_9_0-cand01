//____________________________________________________________________________
/*!

   >>> FOR THE TIME BEING, JUST THE SKELETON FOR ADAPTING VALOR CODE <<<

\class    genie::tune::GFitterGSL

\brief    A GSL-based GENIE fitter

           Algorithm name                                 | GSL fitter option
          ....................................................................
           Simulated annealing                            | SimAnneal
           Levemberg-Marquardt                            | LevMarq
           Fletcher-Reeves conjugate gradient             | ConjugateFR
           Polak-Ribiere conjugate gradient               | ConjugatePR
           Vector Broyden-Fletcher-Goldfarb-Shanno (BFGS) | VectorBFGS
           Steepest Descent                               | SteepestDescent

\reference
          www.gnu.org/software/gsl/manual/html_node/Multidimensional-Minimization.html

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

#ifndef _genie_fitter_gsl_h_
#define _genie_fitter_gsl_h_

#include "Tuning/GFitterABC.h"

namespace genie {
namespace tune {

class GFitterGSL: public GFitterABC
{
public:
   GFitterGSL();
   virtual ~GFitterGSL();

   // implement the FitterABC interface
   bool       DoTheFit      (void)                   { return false; }
   int        FitStatusCode (Option_t * /*op = ""*/) { return 1;     }
   double     FitTime       (void)                   { return 0;     }
   int        FitNTries     (void)                   { return 0;     }
   int        NDOF          (void)                   { return 0;     }
   int        NBinsInFit    (void)                   { return 0;     }
   int        NFitParams    (void)                   { return 0;     }
   void       SetTol        (double /*tol*/)         {               }
   void       SetStr        (int /*str*/   )         {               }
};

} // tune
} // genie

#endif

