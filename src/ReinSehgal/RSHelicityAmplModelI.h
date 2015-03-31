//____________________________________________________________________________
/*!
\class    genie::RSHelicityAmplModelI

\brief    Pure abstract base class. Defines the RSHelicityAmplModelI interface.

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          University of Liverpool & STFC Rutherford Appleton Lab

\created  July 10, 2004

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _REIN_SEHGAL_HELICITY_AMPL_MODEL_I_H_
#define _REIN_SEHGAL_HELICITY_AMPL_MODEL_I_H_

#include "Algorithm/Algorithm.h"
#include "BaryonResonance/BaryonResonance.h"
#include "ReinSehgal/FKR.h"
#include "ReinSehgal/RSHelicityAmpl.h"

namespace genie {

class RSHelicityAmplModelI : public Algorithm
{
public:
  virtual ~RSHelicityAmplModelI();

  // define the RSHelicityAmplModelI interface
  virtual const RSHelicityAmpl & Compute(Resonance_t res, const FKR & fkr) const = 0;

protected:
  RSHelicityAmplModelI();
  RSHelicityAmplModelI(string name);
  RSHelicityAmplModelI(string name, string config);
};

}        // namespace

#endif   // _REIN_SEHGAL_HELICITY_AMPL_MODEL_I_H_



