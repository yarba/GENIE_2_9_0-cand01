//____________________________________________________________________________
/*!

\class    genie::tune::GFitParamList

          NOTE TO SELF: This will interact or get merged with existing GSystSet

\brief    List of parameters included in the fit

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

#ifndef _genie_fit_param_list_h_
#define _genie_fit_param_list_h_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <TNamed.h>
#include <TObjString.h>
#include <TVirtualFitter.h>

// FIXME !!!
// ---> for future --> #include <Minuit2/MnUserParameters.h>

#include <Math/Minimizer.h>
#include <TMatrixDSym.h>
#include <Fit/Fitter.h>

#include "ReWeight/GSyst.h"

using std::string;
using std::map;

using namespace genie::rew;

namespace genie {
namespace tune {

class GFitParamList: public TNamed {

static const int kASmallNum = 1E-8;

public:
   GFitParamList();
   GFitParamList(int nparams);
   GFitParamList(const GFitParamList & fp);
  ~GFitParamList();

   // Get information for the fit parameters
   int    Size                (void)      const { return fNParams;  }
   string Name                (GSyst_t p) const { return fName[this->Id(p)].GetString().Data();   }
   double BestFitValue        (GSyst_t p) const { return fBestFitValue[this->Id(p)]; }
   double BestFitValueByIndex (int i    ) const { return fBestFitValue[i];           }
   double CurrValue           (GSyst_t p) const { return fCurrValue   [this->Id(p)]; }
   double CurrValueByIndex    (int i    ) const { return fCurrValue   [i];           }
   double InitValue           (GSyst_t p) const { return fInitValue   [this->Id(p)]; }
   double MinValue            (GSyst_t p) const { return fMinValue    [this->Id(p)]; }
   double MaxValue            (GSyst_t p) const { return fMaxValue    [this->Id(p)]; }
   double StepSize            (GSyst_t p) const { return fStepSize    [this->Id(p)]; }
   bool   IsFixed             (GSyst_t p) const { return fIsFixed     [this->Id(p)]; }
   bool   IsNuisance          (GSyst_t p) const { return fIsNuisance  [this->Id(p)]; }
   double MigradError         (GSyst_t p) const { return fMigradError [this->Id(p)]; }
   double HesseError          (GSyst_t p) const { return fHesseError  [this->Id(p)]; }
   double PlusError           (GSyst_t p) const { return fPlusError   [this->Id(p)]; }
   double MinusError          (GSyst_t p) const { return fMinusError  [this->Id(p)]; }

   void SetMinValue (GSyst_t p, double min) { fMinValue [this->Id(p)] = min; }
   void SetMaxValue (GSyst_t p, double max) { fMaxValue [this->Id(p)] = max; }

   // Add a parameter
   bool AddParam (int ip, GSyst_t p, double init, double min, double max, double step, bool fix=false, bool nuis=false, string name="");
      
   // Check whether the input parameter was included in the physics param list
   bool Exists (GSyst_t p) const;

   // Update current/running value for the given parameter
   void Update(GSyst_t p, double v);

   // Allow modifying the initial value (minimization may be sensitive to this choice)
   void SetInitValue(GSyst_t p, double v);

   // Reset all current param values to their specified initial values
   void ResetCurrToInitValues (void);

   // Copy current param values to best-fit values & vice-versa
   void CopyCurrToBestFitValues (void);
   void CopyBestFitToCurrValues (void);
   void CopyBestFitToInitValues (void);
   void CopyBestFitToInitValues (const GFitParamList & fp);

   // Copy from input
   using TNamed::Copy; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
   void Copy (const GFitParamList & fp);

   // Set the covariance matrix
   void SetCovMatrix(TMatrixDSym & covmatrix);

   // Set the errors
   void SetErrors(double * errplus, double * errminus, double * errmigrad);
   
   // used with FitterMinuit, vector must be same size as FitParamList->Size()
   void SetMigradErrors(std::vector<double> err);
   void SetHesseErrors (std::vector<double> err);
   void SetMinosErrors (std::vector<double> plus, std::vector<double> minus);

   // Fix a parameter
   void FixParameter        (GSyst_t p); 
   void FixParameterByIndex (int ip); 

   // Free a parameter
   void FreeParameter        (GSyst_t p); 
   void FreeParameterByIndex (int ip); 

   // Set as nuisance parameter (contributes to \chi^{2}_{penalty})
   void SetNuisance (GSyst_t p, bool tf);

   // Create a new fit parameter list that includes only the free parameters of the original list
   GFitParamList * FreeParameterSubset(void);

   // Set parameter list using minuit, minuit2, GSL fitter interfaces
   void SetTo (TVirtualFitter * fitter);
/*
   void SetTo (ROOT::Minuit2::MnUserParameters & params);
   void SetTo (ROOT::Math::Minimizer * minimizer);
   void SetTo (ROOT::Fit::Fitter * fitter);
*/
   // Sync fit param values stored this object with the values stored in the input array.
   void Sync (const double * par);
   void Sync (std::vector<double> par);

   // Get number of free (not fixed) parameters
   int GetNumberFreeParameters (void);

   // Get number of all params (free + fixed)
   int GetNumberParameters (void);

   // Set and get other fit properties: statistic at best-fit point, fit time, status codes etc
   void   ClearStatusCodes    (void);
   void   SetStatusCode       (string st, int code);
   int    GetStatusCode       (string st) const;
   void   SetBestFitStatistic (double stat_bf)   { fBestFitStatistic = stat_bf; }
   double GetBestFitStatistic (void) const       { return fBestFitStatistic;    }
   void   SetCurrStatistic    (double stat_curr) { fCurrStatistic = stat_curr;  }
   double GetCurrStatistic    (void) const       { return fCurrStatistic;       }
   void   SetFitTime          (double fit_time)  { fFitTime = fit_time;         }
   double GetFitTime          (void) const       { return fFitTime;             }
   void   SetFitTries         (int   fit_tries)  { fFitTries = fit_tries;       }
   int    GetFitTries         (void) const       { return fFitTries;            }
   
   TMatrixDSym * GetCovMatrix(void) { return fCovMatrix; }

   // Print
   //using TNamed::Print; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
   void Print (std::ostream & stream) const;
   void Print (Option_t * option = "") const;
   friend std::ostream & operator << (std::ostream & stream, const GFitParamList & plst);

   // Methods with an one-off effect in Print()
   void ShowOnlyCurrValuesInNextPrint   (void) const { fShowCurrValuesOnly    = true; }
   void ShowOnlyBestFitValuesInNextPrint(void) const { fShowBestFitValuesOnly = true; }

   // Get the name used in NamedParameterList
   string GetParListName  (void) const { return fParListName; }
   int    GetSystMode     (void) const { return fSystMode;     }

   // Are the errors set?
   bool MigradErrorsSet (void) const { return fMigradErrorsSet; }
   bool HesseErrorsSet  (void) const { return fHesseErrorsSet;  }
   bool MinosErrorsSet  (void) const { return fMinosErrorsSet;  }

   // mapping from GSyst_t --> array slot
   int Id (GSyst_t p) const; 

   // inverse mapping from array slot --> GSyst_t
   GSyst_t WhichParam(int id) const; 
   
   void SetParListName (string s)   { fParListName = s; }
   void SetSystMode    (int m)      { fSystMode    = m; }
   
private:

   void CleanUp (void);
   void Init    (int nparams);
   
   //
   // Private data members
   //

   map<GSyst_t, int> fIdMap;

   string       fParListName;
   int          fSystMode; // used in FitInputs to make deciding on what to include in the chisq penalty faster. 0 no syst, 1 all syst, 2 selected syst, ...

   int          fNParams;   
   TObjString * fName;         //[fNParams]
   double *     fBestFitValue; //[fNParams]
   double *     fCurrValue;    //[fNParams]
   double *     fInitValue;    //[fNParams]
   double *     fMinValue;     //[fNParams]
   double *     fMaxValue;     //[fNParams]
   double *     fStepSize;     //[fNParams]
   bool *       fIsFixed;      //[fNParams]
   bool *       fIsNuisance;   //[fNParams]
   double *     fMigradError;  //[fNParams]
   double *     fHesseError;   //[fNParams]
   double *     fPlusError;    //[fNParams]
   double *     fMinusError;   //[fNParams]
   TMatrixDSym* fCovMatrix;  
   double       fBestFitStatistic; 
   double       fCurrStatistic; 
   double       fFitTime;
   int          fFitTries;

   bool fMigradErrorsSet;
   bool fHesseErrorsSet;
   bool fMinosErrorsSet;

   map<string, int> fStatusCodes;

   mutable bool fShowCurrValuesOnly;
   mutable bool fShowBestFitValuesOnly;

ClassDef(GFitParamList,1)
};

std::ostream & operator<< (std::ostream & stream, const GFitParamList & plst);

} // tune
} // genie
  
#endif


