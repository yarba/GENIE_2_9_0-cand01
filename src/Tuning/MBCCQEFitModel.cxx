//____________________________________________________________________________
/*
 Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Lab
 
 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________

#include <TH2D.h>
#include <TMath.h>

#include "EVGCore/EventRecord.h"
#include "Messenger/Messenger.h"
#include "ReWeight/GReWeight.h"
#include "ReWeight/GReWeightNuXSecCCQE.h"
#include "ReWeight/GSystSet.h"
#include "ReWeight/GSyst.h"

#include "Tuning/MBCCQEFitModel.h"

using namespace genie;
using namespace genie::rew;
using namespace genie::tune;

//____________________________________________________________________________
MBCCQEFitModel::MBCCQEFitModel() :
GExFitModelABC(),
MBCCQEModel()
{
  // ----------------------------------- needs improvement / start
  // Should not have a GReWeight object within in each fit model.
  fRwght = new GReWeight; 
  // Will a general way to configure and include weight calculators based on
  // the kind of fit that gets performed
  GReWeightNuXSecCCQE * rwccqe = new GReWeightNuXSecCCQE;
  rwccqe->SetMode(GReWeightNuXSecCCQE::kModeMa);
  fRwght->AdoptWghtCalc("xsec_ccqe", rwccqe);
  // Will need a better way to communicate information from GFitParamList to
  // GSystSet. Maybe consolidate the two to a single class or, perhaps better,
  // make GFitParamList (which includes much more functionality) to spawn a 
  // GSystSet object. For now, hardcode a single param.
  GSystSet & syst = fRwght->Systematics();
  syst.Init(kXSecTwkDial_MaCCQE);
  //------------------------------------ needs improvement / end

//  fMBCCQEModel = new MBCCQEModel;
}
//____________________________________________________________________________
MBCCQEFitModel::~MBCCQEFitModel()
{
  delete fRwght;
//  delete fMBCCQEModel;
}
//____________________________________________________________________________
string MBCCQEFitModel::Name(void)
{
  return MBCCQEModel::Name();
//  return fMBCCQEModel->Name();
}
//____________________________________________________________________________
bool MBCCQEFitModel::Read(string file, Option_t * opt)
{
  return MBCCQEModel::Read(file,opt); 
//  return fMBCCQEModel->Read(file,opt); 
} 
//____________________________________________________________________________ 
bool MBCCQEFitModel::Generate(Option_t * opt) 
{ 
  return MBCCQEModel::Generate(opt); 
//  return fMBCCQEModel->Generate(opt); 
} 
//____________________________________________________________________________ 
bool MBCCQEFitModel::Write(
  TObjArray* collection, const char * tag, Option_t * write_opt)
{
  return MBCCQEModel::Write(collection,tag,write_opt); 
//  return fMBCCQEModel->Write(collection,tag,write_opt); 
}
//____________________________________________________________________________
bool MBCCQEFitModel::Write(
  const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{
  return MBCCQEModel::Write(filename,tag,write_opt,file_opt); 
//  return fMBCCQEModel->Write(filename,tag,write_opt,file_opt); 
}
//____________________________________________________________________________
double MBCCQEFitModel::Weight(EventRecord & event)
{
// Use event reweighting and override the implementation in MBCCQEModel

  return fRwght->CalcWeight(event);
}
//____________________________________________________________________________
bool MBCCQEFitModel::Generate(const GFitParamList & fp, Option_t * opt)
{
  // ----------------------------------- needs improvement / start
  // Handle the general case - any combination of parameters supported by reweighting
  // Also, GENIE should be reconfigured by the fitter once in every iteration 
  // in the minimization loop, not every time the Generate() method is invoked.
  double ma = fp.CurrValue(kXSecTwkDial_MaCCQE);   
  LOG("MiniBooNE", pNOTICE) << "Trying x_MaCCQE = " << ma;
  GSystSet & syst = fRwght->Systematics();
  syst.Set(kXSecTwkDial_MaCCQE, ma);
  fRwght->Reconfigure();
  //------------------------------------ needs improvement / end

  return MBCCQEModel::Generate(opt);
}
//____________________________________________________________________________
double MBCCQEFitModel::Statistic(
  GExDataABC * data, const GFitParamList & fp, Option_t * opt)
{
  //------------------------------------ needs improvement 
  // Ignoring  input option and calculating a simple chisq statistic
  //
  // Also calculating chisq using histograms 
  // But the intention is to use histograms only for visialization as it is
  // much more CPU efficient not to generate histograms in every step and do
  // the calculation with the basic arrays
  //-------------------------------------------------------        

  // generate prediction for input set of params
  this->Generate(fp); // Here: eventually set option to fill-in only the arrays in this step
          
  TObjArray * data_array = new TObjArray();
  data->Write(data_array,"data","all");
  const char * data_name  = Form("%s_%s_d2sig","data",this->Name().c_str());
  TH2D * data_h2 = (TH2D*) data_array->FindObject(data_name);
  assert(data_h2);

  TObjArray * model_array = new TObjArray();
  this->Write(model_array,"model","all");
  const char * model_name  = Form("%s_%s_d2sig","model",this->Name().c_str());
  TH2D * model_h2 = (TH2D*) model_array->FindObject(model_name);
  assert(model_h2);
 
  double chisq = 0;
  
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
  
        double data_ij = data_h2->GetBinContent(i+1,j+1); // HERE: switch to using the actual array, not TH2D
        if(data_ij <= 0.) continue;

        double mc_ij = model_h2->GetBinContent(i+1,j+1); // HERE: switch to using the actual array, not TH2D

/* 
HERE: make sure data->Write() also writes out the errors
        double sig_shape_ij  = fdDiffXSecShapeUncert [i][j] * CCQE::kNuUncertScaling; // Only for the Nu 2010 data release
        double sig_norm_ij   = CCQE::kNuNormUncert * data_ij; //
        double sig_ij_square = sig_shape_ij*sig_shape_ij + sig_norm_ij*sig_norm_ij;
*/
        double sig_ij_square = TMath::Power(0.1*data_ij,2); // HERE: temp
 
        double delta_chisq = TMath::Power(data_ij - mc_ij, 2.) / sig_ij_square;
  
        LOG("MiniBooNE", pNOTICE)
          << "data = " << data_ij << ", err = " << TMath::Sqrt(sig_ij_square)
          << ", prediction = " << mc_ij << ", \\delta\\chi^2 =  " << delta_chisq;
      
        chisq += delta_chisq;
    }//j
  }//i
 
  LOG("MiniBooNE", pNOTICE) << "\\chi^2 = " << chisq;

  return chisq;
}
//____________________________________________________________________________

