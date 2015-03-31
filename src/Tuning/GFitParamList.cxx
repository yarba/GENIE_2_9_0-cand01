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

#include <cassert>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>

#include <TSystem.h>

#include "Tuning/GFitParamList.h"

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::setprecision;

using namespace genie;
using namespace genie::tune;

ClassImp(GFitParamList)

static const int kASmallNum = 1E-8;

//________________________________________________________________________________________
namespace genie {
 namespace tune {
  ostream & operator << (ostream & stream, const GFitParamList & plst)
  {
    plst.Print(stream);
    return stream;
  }
 }
}
//________________________________________________________________________________________
GFitParamList::GFitParamList() :
TNamed()
{
  this->Init(0);
}
//________________________________________________________________________________________
GFitParamList::GFitParamList(int nparams) :
TNamed()
{
  this->Init(nparams);
}
//________________________________________________________________________________________
GFitParamList::GFitParamList(const GFitParamList & plst) :
TNamed()
{
  this->Init(plst.fNParams);
  this->Copy(plst);
}
//________________________________________________________________________________________
GFitParamList::~GFitParamList()
{
  this->CleanUp();
}

//________________________________________________________________________________________
bool GFitParamList::AddParam (int ip, GSyst_t p, 
  double init, double min, double max, double step, bool fix, bool nuis, string name)
{
  if (ip < 0 || ip >= fNParams) {
    this->Print();
    cerr << "FitParamList/AddParam >> Failed to add parameter "
         << GSyst::AsString(p) << " (" <<name
         << ") to slot "<<ip<<" (max params " << fNParams << ") "<<endl;
    exit(-1);
  }

  fIdMap.insert(map<GSyst_t, int>::value_type(p,ip));

  if(name.size()==0) {
    name = GSyst::AsString(p);
  }

  fName         [ip].SetString(name.c_str());
  fBestFitValue [ip] = -99999999;
  fCurrValue    [ip] = init;
  fInitValue    [ip] = init;
  fMinValue     [ip] = min; 
  fMaxValue     [ip] = max; 
  fStepSize     [ip] = step; 
  fIsFixed      [ip] = fix;  
  fIsNuisance   [ip] = nuis;  

  return true;
}
//________________________________________________________________________________________
bool GFitParamList::Exists(GSyst_t p) const
{
  map<GSyst_t, int>::const_iterator iter = fIdMap.find(p);
  if(iter != fIdMap.end()) return true;
  return false;
}
//________________________________________________________________________________________
void GFitParamList::Update(GSyst_t p, double v)
{
  fCurrValue [this->Id(p)] = v;
}
//________________________________________________________________________________________
void GFitParamList::SetInitValue(GSyst_t p, double v)
{
  fInitValue [this->Id(p)] = v;
  fCurrValue [this->Id(p)] = v;
}                  
//________________________________________________________________________________________
void GFitParamList::ResetCurrToInitValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = fInitValue[ip];
  }
}
//________________________________________________________________________________________
void GFitParamList::CopyCurrToBestFitValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fBestFitValue[ip] = fCurrValue[ip];
  }
  fBestFitStatistic = fCurrStatistic;
}
//________________________________________________________________________________________
void GFitParamList::CopyBestFitToCurrValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = fBestFitValue[ip];
  }
  fCurrStatistic = fBestFitStatistic;

}
//________________________________________________________________________________________
void GFitParamList::CopyBestFitToInitValues(void)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fInitValue[ip] = fBestFitValue[ip];
  }
}
//________________________________________________________________________________________
void GFitParamList::CopyBestFitToInitValues(const GFitParamList & plst)
{
  if(plst.GetParListName() != fParListName) {
    cerr << "Cannot copy best fit values in fit param list: " << plst.GetParListName()
	 << " to init values in fit param list: " << fParListName << endl;
    exit(1);
  }
  for(int ip = 0; ip < fNParams; ip++) {
    fInitValue[ip] = plst.BestFitValueByIndex(ip);
  }
}
//________________________________________________________________________________________
void GFitParamList::SetCovMatrix(TMatrixDSym & covmatrix)
{
  TMatrixDSym & m = *fCovMatrix;
  for(int ip = 0; ip < fNParams; ip++) {
    for(int jp = 0; jp < fNParams; jp++) {
       m[ip][jp] = covmatrix[ip][jp];
    }
  }
}
//________________________________________________________________________________________
void GFitParamList::SetErrors(double * errplus, double * errminus, double * errmigrad)
{
  for(int ip = 0; ip < fNParams; ip++) {
     fPlusError  [ip] = errplus  [ip];
     fMinusError [ip] = errminus [ip];
     fMigradError[ip] = errmigrad[ip];
     if(errplus[ip] > kASmallNum || errminus[ip] > kASmallNum)
       fMinosErrorsSet = true;
  }
  fMigradErrorsSet = true;
}
//________________________________________________________________________________________
void GFitParamList::SetMigradErrors(vector<double> err)
{
  if(err.size() != (unsigned int)this->Size()) {
    cerr << "FitterKernelMinuit2/SetMigradErrors >> Error vector size: " << err.size()
	 << " is not the same size as the number of free parameters: "
	 << this->GetNumberFreeParameters() << endl;
    exit(1);
  }
  
  for(int ip = 0; ip < fNParams; ip++) {
    fMigradError [ip] = err[ip];
  }
  fMigradErrorsSet = true;
}
//________________________________________________________________________________________
void GFitParamList::SetHesseErrors(vector<double> err)
{
  if(err.size() != (unsigned int)this->Size()) {
    cerr << "FitterKernelMinuit2/SetHesseErrors >> Error vector size: " << err.size()
	 << " is not the same size as the number of free parameters: "
	 << this->GetNumberFreeParameters() << endl;
    exit(1);
  }
  
  for(int ip = 0; ip < fNParams; ip++) {
    fHesseError [ip] = err[ip];
  }
  fHesseErrorsSet = true;
}
//________________________________________________________________________________________
void GFitParamList::SetMinosErrors(vector<double> plus, vector<double> minus)
{
  if((int)plus.size() != this->Size()) {
    cerr << "FitterKernelMinuit2/SetMinosErrors >> Error vector size: " << plus.size()
	 << " is not the same size as the number of parameters: "
	 << this->GetNumberFreeParameters() << endl;
    exit(1);
  }
  if(plus.size() != minus.size()) {
    cerr << "FitterKernelMinuit2/SetMinosErrors >> Error vector size (plus): " << plus.size()
	 << " is not the same size as error vector size (minus): " << minus.size() << endl;
    exit(1);
  }

  for(int ip = 0; ip < fNParams; ip++) {
    fPlusError [ip] = plus [ip];
    fMinusError[ip] = minus[ip];
  }
  
  fMinosErrorsSet = true;
}
//________________________________________________________________________________________
void GFitParamList::FixParameter(GSyst_t p)
{
  fIsFixed [this->Id(p)] = true;
}
//________________________________________________________________________________________
void GFitParamList::FixParameterByIndex(int ip)
{
  fIsFixed [ip] = true;
}
//________________________________________________________________________________________
void GFitParamList::FreeParameter(GSyst_t p)
{
  fIsFixed [this->Id(p)] = false;
}
//________________________________________________________________________________________
void GFitParamList::FreeParameterByIndex(int ip)
{
  fIsFixed [ip] = false;
}
//________________________________________________________________________________________
void GFitParamList::SetNuisance(GSyst_t p, bool tf)
{
  fIsNuisance [this->Id(p)] = tf;
}
//________________________________________________________________________________________
GFitParamList * GFitParamList::FreeParameterSubset(void)
{
  int nfree = this->GetNumberFreeParameters();

  GFitParamList * fparm = new GFitParamList(nfree);

  int ipc = 0;
  for(int ip = 0; ip < fNParams; ip++) {
    if(fIsFixed[ip]) continue;
    GSyst_t p = this->WhichParam(ip);
    double init = fInitValue[ip];
    double min  = fMinValue[ip];
    double max  = fMaxValue[ip];
    double step = fStepSize[ip];
    bool   nuis = fIsNuisance[ip];
    fparm->AddParam(ipc, p, init, min, max, step, false, nuis);
    ipc++;
  }

  return fparm;
}
//________________________________________________________________________________________
void GFitParamList::SetTo(TVirtualFitter * fitter)
{
  if(!fitter) return;
  fitter->Clear();
  map<GSyst_t, int>::const_iterator iter = fIdMap.begin();
  for ( ; iter != fIdMap.end(); ++iter) {
    GSyst_t p = iter->first;
    int iparam = iter->second;
    fitter->SetParameter(
      iparam,
      this->Name(p).c_str(),
      this->InitValue(p),
      this->StepSize(p),
      this->MinValue(p),
      this->MaxValue(p) );

    if(this->IsFixed(p)) {
     fitter->FixParameter(iparam);
    }
  }
}
//________________________________________________________________________________________
void GFitParamList::Sync(std::vector<double> par)
{
  if(par.size() <= 0) return;
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = par[ip];
  }
}
//________________________________________________________________________________________
void GFitParamList::Sync(const double * par)
{
  if(!par) return;
  for(int ip = 0; ip < fNParams; ip++) {
     fCurrValue[ip] = par[ip];
  }
}
//________________________________________________________________________________________
int GFitParamList::GetNumberFreeParameters(void)
{
  int nparams = 0;

  for(int ip=0; ip<fNParams;  ip++) {
    bool is_fixed = fIsFixed[ip];
    if(!is_fixed) {
      nparams++;
    }
  }
  return nparams;
}
//________________________________________________________________________________________
int GFitParamList::GetNumberParameters(void)
{
  int nparams = 0;

  for(int ip=0; ip<fNParams;  ip++) {
    nparams++;
  }
  return nparams;
}
//________________________________________________________________________________________
void GFitParamList::ClearStatusCodes(void)
{
  fStatusCodes.clear();
}
//________________________________________________________________________________________
void GFitParamList::SetStatusCode(string st, int code)
{
  fStatusCodes[st] = code;
}
//________________________________________________________________________________________
int GFitParamList::GetStatusCode(string st) const
{
  map<string,int>::const_iterator sciter = fStatusCodes.find(st);
  if(sciter != fStatusCodes.end()) {
    return sciter->second;
  }
  return -9876;
}
//________________________________________________________________________________________
void GFitParamList::Print(ostream & stream) const
{
  if(fShowBestFitValuesOnly) {
     fShowBestFitValuesOnly = false; // has one-off effect
     stream << endl
            << setw(6)  << "id"
            << setw(80) << "name"
            << setw(15) << "best-fit"
            << endl;
     for(int ip=0; ip<fNParams;  ip++) {
       stream << setw(6)  << ip 
              << setw(80) << fName[ip].GetString().Data()
              << setprecision(6)
              << setw(15) << fBestFitValue[ip]
              << endl;
     }
  } 
  else
  if(fShowCurrValuesOnly) {
     fShowCurrValuesOnly = false; // has one-off effect
     stream << endl
            << setw(6)  << "id"
            << setw(80) << "name"
            << setw(15) << "curr"
            << endl;
     for(int ip=0; ip<fNParams;  ip++) {
       stream << setw(6)  << ip 
              << setw(80) << fName[ip].GetString().Data()
              << setprecision(6)
              << setw(15) << fCurrValue[ip]
              << endl;
     }
  } 
  else 
  {
     stream << endl
            << setw(6)  << "id"
            << setw(35) << "name"
            << setw(15) << "curr"
            << setw(15) << "init"
            << setw(15) << "min"
            << setw(15) << "max"
            << setw(15) << "step"
            << setw(8)  << "fixed?"
            << setw(10) << "nuisance?"
            << endl;
     for(int ip=0; ip<fNParams;  ip++) {
       stream << setw(6)  << ip 
              << setw(35) << fName[ip].GetString().Data()
              << setprecision(6)
              << setw(15) << fCurrValue[ip]
              << setw(15) << fInitValue[ip]
              << setw(15) << fMinValue[ip]
              << setw(15) << fMaxValue[ip]
              << setw(15) << fStepSize[ip]
              << setw(8) << ((fIsFixed[ip])    ? "yes" : "no")
              << setw(8) << ((fIsNuisance[ip]) ? "yes" : "no")
              << endl;
     }
  }

  stream << "Status codes: " << endl;  
  map<string,int>::const_iterator sciter = fStatusCodes.begin();
  for( ; sciter != fStatusCodes.end(); ++sciter) {
    string name = sciter->first;
    int code = sciter->second;
    stream << setw(20) << name << setw(10) << code << endl;  
  }
  stream << "Fit time: " << fFitTime  << " sec" << ", tries: " << fFitTries << endl;
  stream << "Best-fit statistic: " << fBestFitStatistic << endl;  
}
//________________________________________________________________________________________
void GFitParamList::Print(Option_t *) const
{
  this->Print(cout);
}
//________________________________________________________________________________________
void GFitParamList::Copy(const GFitParamList & plst)
{
  if(this->fNParams != plst.fNParams)
  {
     this->CleanUp();
     this->Init(plst.fNParams);
  }

  this->fParListName = plst.fParListName;
  this->fSystMode    = plst.fSystMode;

  this->fIdMap.insert(plst.fIdMap.begin(),  plst.fIdMap.end());

  for(int i = 0; i < plst.fNParams; i++) {
     this->fName[i].SetString(plst.fName[i].GetString().Data());
     this->fBestFitValue[i] = plst.fBestFitValue[i];
     this->fCurrValue[i]    = plst.fCurrValue[i];
     this->fInitValue[i]    = plst.fInitValue[i];
     this->fMinValue[i]     = plst.fMinValue[i];
     this->fMaxValue[i]     = plst.fMaxValue[i];
     this->fStepSize[i]     = plst.fStepSize[i];
     this->fIsFixed[i]      = plst.fIsFixed[i];
     this->fIsNuisance[i]   = plst.fIsNuisance[i];
     this->fMigradError[i]  = plst.fMigradError[i];
     this->fHesseError[i]   = plst.fHesseError[i];
     this->fPlusError[i]    = plst.fPlusError[i];
     this->fMinusError[i]   = plst.fMinusError[i];
  } 

  for(int i = 0; i < plst.fNParams; i++) {
    for(int j = 0; j < plst.fNParams; j++) {
      (*this->fCovMatrix)[i][j] =  (*plst.fCovMatrix)[i][j];
    }
  }

   this->fBestFitStatistic = plst.fBestFitStatistic;
   this->fCurrStatistic    = plst.fCurrStatistic;
   this->fFitTime          = plst.fFitTime;
   this->fFitTries         = plst.fFitTries;

   this->fStatusCodes.insert(plst.fStatusCodes.begin(),  plst.fStatusCodes.end());

   this->fMigradErrorsSet = plst.fMigradErrorsSet;
   this->fHesseErrorsSet  = plst.fHesseErrorsSet;
   this->fMinosErrorsSet  = plst.fMinosErrorsSet;

   this->fShowCurrValuesOnly    = plst.fShowCurrValuesOnly;
   this->fShowBestFitValuesOnly = plst.fShowBestFitValuesOnly;
}
//________________________________________________________________________________________
void GFitParamList::Init(int nparams)
{
   fIdMap.clear();

   fParListName = "";
   fSystMode = -1;

   fNParams = nparams; 

   if(nparams>0) {
     fName         = new TObjString[fNParams];
     fBestFitValue = new double[fNParams];
     fCurrValue    = new double[fNParams];
     fInitValue    = new double[fNParams];
     fMinValue     = new double[fNParams];
     fMaxValue     = new double[fNParams];
     fStepSize     = new double[fNParams];
     fIsFixed      = new bool  [fNParams];
     fIsNuisance   = new bool  [fNParams];
     fMigradError  = new double[fNParams];
     fHesseError   = new double[fNParams];
     fPlusError    = new double[fNParams];
     fMinusError   = new double[fNParams];
     fCovMatrix    = new TMatrixDSym(fNParams);
   } else {
     fName         = 0;
     fBestFitValue = 0;
     fCurrValue    = 0;
     fInitValue    = 0;
     fMinValue     = 0;
     fMaxValue     = 0;
     fStepSize     = 0;
     fIsFixed      = 0;
     fIsNuisance   = 0;
     fMigradError  = 0;
     fHesseError   = 0;
     fPlusError    = 0;
     fMinusError   = 0;
     fCovMatrix    = 0;
   }
   
   for (int i = 0 ; i < fNParams ; i++) {
     fName         [i].SetString("Uninitialised");
     fBestFitValue [i] = 0;
     fCurrValue    [i] = 0;
     fInitValue    [i] = 0;
     fMinValue     [i] = 0;
     fMaxValue     [i] = 0;
     fStepSize     [i] = 0;
     fIsFixed      [i] = 0;
     fIsNuisance   [i] = 0;
     fMigradError  [i] = 0;
     fHesseError   [i] = 0;
     fPlusError    [i] = 0;
     fMinusError   [i] = 0;
   }
     

   fShowCurrValuesOnly    = false;
   fShowBestFitValuesOnly = false;

   fBestFitStatistic = 0.;
   fCurrStatistic    = 0.;
   fFitTime          = -1;
   fFitTries         = 0;

   fStatusCodes.clear();

   fMigradErrorsSet = false;
   fHesseErrorsSet  = false;
   fMinosErrorsSet  = false;
}
//________________________________________________________________________________________
void GFitParamList::CleanUp(void)
{
   fIdMap.clear();
   fStatusCodes.clear();

   delete [] fName;
   delete [] fBestFitValue;
   delete [] fCurrValue;
   delete [] fInitValue;
   delete [] fMinValue;
   delete [] fMaxValue;
   delete [] fStepSize;
   delete [] fIsFixed;
   delete [] fIsNuisance;
   delete [] fMigradError;
   delete [] fHesseError;
   delete [] fPlusError;
   delete [] fMinusError;
   delete fCovMatrix;

   fName         = 0;
   fBestFitValue = 0;
   fCurrValue    = 0;
   fInitValue    = 0;
   fMinValue     = 0;
   fMaxValue     = 0;
   fStepSize     = 0;
   fIsFixed      = 0;
   fIsNuisance   = 0;
   fMigradError  = 0;
   fHesseError   = 0;
   fPlusError    = 0;
   fMinusError   = 0;
   fCovMatrix    = 0;

   fBestFitStatistic = 0.;
   fCurrStatistic    = 0.;
   fFitTime          = 0.;
   fFitTries         = -1;

   fParListName = "";
   fSystMode    = -1;
}
//________________________________________________________________________________________
int GFitParamList::Id(GSyst_t p) const
{
   map<GSyst_t, int>::const_iterator it = fIdMap.find(p);
   if(it == fIdMap.end()) {
     cerr << "GFitParamList::Id() >> Parameter: " << GSyst::AsString(p) << " GSyst_t p = " << p 
          << " was not included in current fit parameter list" << endl;
     exit(1);
   }
   int id = it->second;
   return id;
}
//________________________________________________________________________________________
GSyst_t GFitParamList::WhichParam(int id) const
{
   map<GSyst_t, int>::const_iterator it = fIdMap.begin();
   for( ; it != fIdMap.end(); ++it) {
      int curr_id = it->second;
      if(curr_id == id) return it->first;
   }
   return kNullSystematic;
}
//________________________________________________________________________________________
