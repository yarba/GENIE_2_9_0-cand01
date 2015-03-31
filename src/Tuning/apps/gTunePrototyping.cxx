//___________________________________________________________________________________________
/*!

\program gtune_proto

\brief   
               
             
\author  Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Laboratory

         Chris Barry <cbarry \at hep.ph.liv.ac.uk>
         University of Liverpool

\created Feb 05, 2015 

\cpright Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
         For the full text of the license visit http://copyright.genie-mc.org
         or see $GENIE/LICENSE
*/
//___________________________________________________________________________________________

#include <iostream>

#include "Messenger/Messenger.h"
#include "ReWeight/GSyst.h"
#include "DataComp/Base/GExDataABC.h"
#include "DataComp/Base/GExModelABC.h"
#include "Tuning/GFitterABC.h"
#include "Tuning/GFitterMinuit.h"
#include "Tuning/GFitParamList.h"
#include "Tuning/GFitInputs.h"

// Eventually, I should have a factory and request specific datasets and corresponding models
// by name. This shall return pointers to GExDataABC and GExModelABC base classes.
// Direct includes will not be needed.
#include "DataComp/MiniBooNE/MBCCQEData.h"
#include "DataComp/MiniBooNE/MBCCQEModel.h"
#include "Tuning/MBCCQEFitModel.h"

using namespace genie;
using namespace genie::mc_vs_data;

int main()
{   
  // Instantiate and read/configure the data and model
  GExDataABC *  data  = new MBCCQEData;
  GExModelABC * model = new MBCCQEModel;    // nominal model, no reweighting hook
  //GExModelABC * model = new MBCCQEFitModel; // extends the nominal model, includes reweighting hook
  data  -> Read("/opt/ppd/t2k/softw/GENIE/builds/SL5_64bit/miniboone_fits/data/validation/vA/xsec/differential/miniboone-ccqenu2010.xml");
  model -> Read("blah_2.xml");

  // Generate nominal model prediction 
  model -> Generate(); 

  // Write out the data and nominal predictions
  data  -> Write("mbccqe_fit.root", "data",               "all", "recreate");
  model -> Write("mbccqe_fit.root", "genie_nuVT_nominal", "all", "update");

/*  
  // Instantiate list of fit parameters
  GFitParamList params(1); // 1 param fit  
  // Add params
  //              slot  param                 init  min   max  step   fix?   nuis?  name
  params.AddParam(0,    kXSecTwkDial_MaCCQE,  0.,   -3.,  3.,  0.01,  false, false, "x_MACCQE");
  LOG("TestFitApp", pNOTICE) << params; 

  // Assemble fit inputs and get one of the fitting objects
  GFitInputs * fit_inputs = new GFitInputs(data,model,&params);
  GFitterABC * fitter = new GFitterMinuit(fit_inputs, "minuit+minos_err");

  // Run the fit
  fitter -> DoTheFit();

  // Append the best-fit prediction in the output file
  model -> Write("mbccqe_fit.root", "genie_nuVT_bestfit", "all", "update");
*/
  return 0;
}
