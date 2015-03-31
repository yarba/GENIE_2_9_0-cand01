//________________________________________________________________________________________
/*
 Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
 University of Liverpool & STFC Rutherford Appleton Laboratory

 Adapted from VALOR oscillation fitting group code developed by
 C. Andreopoulos (Liverpool & STFC RAL), T. Dealtry (Oxford),
 S. Dennis (Warwick), L. Escudero (IFIC), N.Grant (Lancaster) and
 D.Sgalaberna (ETHZ).

 For the class documentation see the corresponding header file.

*/
//________________________________________________________________________________________

//#define __show_bin_contributions_to_chisq__
//#define __show_contributions_to_penalty_term__
//#define __show_chisq__

#include <cassert>
#include <cstdlib>
#include <iostream>

#include <TMinuit.h>
#include <TString.h>
#include <TMath.h>
/*
#include "Math/FitMethodFunction.h"
#include "HFitInterface.h"
*/

#include "DataComp/Base/GExDataABC.h"
#include "Tuning/GExFitModelABC.h"
#include "Tuning/GFitParamList.h"
#include "Tuning/GFitInputs.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace genie;
using namespace genie::tune;

ClassImp(GFitInputs)

//________________________________________________________________________________________
GFitInputs::GFitInputs(
 GExDataABC * data, GExFitModelABC *  model, GFitParamList * params) :
fData   (data), 
fModel  (model), 
fParams (params)
{ 

}
//________________________________________________________________________________________
GFitInputs::~GFitInputs()  
{ 

}
//________________________________________________________________________________________
void GFitInputs::UpdateParamList(GFitParamList * fp)
{
  fParams = fp;
}
//________________________________________________________________________________________
GExDataABC & GFitInputs::Data(void) 
{ 
  return *fData;       
}
//________________________________________________________________________________________
GExFitModelABC & GFitInputs::Model(void) 
{ 
  return *fModel;       
}
//________________________________________________________________________________________
GFitParamList & GFitInputs::Params(void) 
{ 
  return *fParams; 
}
//________________________________________________________________________________________
double GFitInputs::Statistic(Option_t * option) const
{
  if (option) {
    return fModel->Statistic(fData,*fParams,option);
  }
  return fModel->Statistic(fData,*fParams, NULL);
}
//____________________________________________________________________________ 
//
// MINUIT fit functions 
// 
//____________________________________________________________________________ 
void genie::tune::FitFunc (
  Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t /*iflag*/)
{
// To be used with a TVirtualFitter

  GFitInputs * inputs = (GFitInputs *) gMinuit->GetObjectFit();

  // get fit params
  GFitParamList & fp = inputs->Params(); 

  // synchronize FitParamList with the paramerer array used by MINUIT
  fp.Sync(par);

  // judge the agreement between the data and the current prediction
  GExDataABC &     data  = inputs->Data();
  GExFitModelABC & model = inputs->Model();

  f = model.Statistic(&data,fp,"likelihood-ratio");
  fp.SetCurrStatistic(f);
}
//____________________________________________________________________________
