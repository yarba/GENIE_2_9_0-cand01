//____________________________________________________________________________
/*
 Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Chris Barry <cbarry \at hep.ph.liv.ac.uk>
         University of Liverpool

 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________

#include <iostream>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TIterator.h>
#include <TH1D.h>

#include "DataComp/MiniBooNE/MBNC1pi0Data.h"
#include "DataComp/MiniBooNE/MBNC1pi0Binning.h"

using namespace genie;
using namespace genie::mc_vs_data;
using namespace genie::mc_vs_data::constants::MB;
//____________________________________________________________________________
MBNC1pi0Data::MBNC1pi0Data(MBNC1pi0Binning & binning)
{
  // Get bin edges arrays for use in rest of class
  // Neutrino mode bin edges arrays
  fNuModeNuPpi0BinEdges             = binning.GetNuModeNuPpi0BinEdges()            ;
  fNuModeNuCosThetapi0BinEdges      = binning.GetNuModeNuCosThetapi0BinEdges()     ;
  fNuModeNuNuBarPpi0BinEdges        = binning.GetNuModeNuNuBarPpi0BinEdges()       ; 
  fNuModeNuNuBarCosThetapi0BinEdges = binning.GetNuModeNuNuBarCosThetapi0BinEdges(); 
  
  // Antineutrino mode bin edges arrays
  fNuBarModePpi0BinEdges               = binning.GetNuBarModePpi0BinEdges()              ;
  fNuBarModeCosThetapi0BinEdges        = binning.GetNuBarModeCosThetapi0BinEdges()       ;  
  fNuBarModeNuNuBarPpi0BinEdges        = binning.GetNuBarModeNuNuBarPpi0BinEdges()       ; 
  fNuBarModeNuNubarCosThetapi0BinEdges = binning.GetNuBarModeNuNuBarCosThetapi0BinEdges(); 

  // Initialise arrays
  // Initialise cross section data bin arrays
  InitialiseArray(fNuModeNuPpi0Xsec            , NC1pi0::kNNuModeNuPpi0Bins            );
  InitialiseArray(fNuModeNuCosThetapi0Xsec     , NC1pi0::kNNuModeNuCosThetapi0Bins     );
  InitialiseArray(fNuModeNuNuBarPpi0Xsec       , NC1pi0::kNNuModeNuNuBarPpi0Bins       );
  InitialiseArray(fNuModeNuNuBarCosThetapi0Xsec, NC1pi0::kNNuModeNuNuBarCosThetapi0Bins);
  
  InitialiseArray(fNuBarModePpi0Xsec              , NC1pi0::kNNuBarModeNuPpi0Bins            );
  InitialiseArray(fNuBarModeCosThetapi0Xsec       , NC1pi0::kNNuBarModeNuCosThetapi0Bins     );
  InitialiseArray(fNuBarModeNuNuBarPpi0Xsec       , NC1pi0::kNNuBarModeNuNuBarPpi0Bins       );
  InitialiseArray(fNuBarModeNuNubarCosThetapi0Xsec, NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins);

  // Initialise covariance matrices 
  InitialiseArray(fNuModeNuPpi0Cov            , ( NC1pi0::kNNuModeNuPpi0Bins             * NC1pi0::kNNuModeNuPpi0Bins            ));
  InitialiseArray(fNuModeNuCosThetapi0Cov     , ( NC1pi0::kNNuModeNuCosThetapi0Bins      * NC1pi0::kNNuModeNuCosThetapi0Bins     ));
  InitialiseArray(fNuModeNuNuBarPpi0Cov       , ( NC1pi0::kNNuModeNuNuBarPpi0Bins        * NC1pi0::kNNuModeNuNuBarPpi0Bins       ));
  InitialiseArray(fNuModeNuNuBarCosThetapi0Cov, ( NC1pi0::kNNuModeNuNuBarCosThetapi0Bins * NC1pi0::kNNuModeNuNuBarCosThetapi0Bins));

  InitialiseArray(fNuBarModePpi0Cov              , ( NC1pi0::kNNuBarModeNuPpi0Bins             * NC1pi0::kNNuBarModeNuPpi0Bins             ));
  InitialiseArray(fNuBarModeCosThetapi0Cov       , ( NC1pi0::kNNuBarModeNuCosThetapi0Bins      * NC1pi0::kNNuBarModeNuCosThetapi0Bins      ));
  InitialiseArray(fNuBarModeNuNuBarPpi0Cov       , ( NC1pi0::kNNuBarModeNuNuBarPpi0Bins        * NC1pi0::kNNuBarModeNuNuBarPpi0Bins        ));
  InitialiseArray(fNuBarModeNuNubarCosThetapi0Cov, ( NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins * NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins ));

  // Path to data in Genie
  //  fNC1pi0Path = ;

  // Cross section data file variables
  fnu_mode_nu_1pi0_Ppi0_xsec   	             = "nuppi0xsec.txt";			  
  fnu_mode_nu_1pi0_CosThetapi0_xsec          = "nucosthetapi0xsec.txt";			  
  fnu_mode_nu_nubar_1pi0_Ppi0_xsec           = "combinedsignnumodeppi0xsec.txt";	  
  fnu_mode_nu_nubar_1pi0_CosThetapi0_xsec    = "combinedsignnumodecosthetapi0xsec.txt";	 
 
  fnubar_mode_nu_1pi0_Ppi0_xsec 	     = "nubarppi0xsec.txt";			  
  fnubar_mode_nu_1pi0_CosThetapi0_xsec       = "nubarcosthetapi0xsec.txt";		  
  fnubar_mode_nu_nubar_1pi0_Ppi0_xsec        = "combinedsignnubarmodeppi0xsec.txt";	  
  fnubar_mode_nu_nubar_1pi0_CosThetapi0_xsec = "combinedsignnubarmodecosthetapi0xsec.txt";

  // Covariance Matrix data files
  fnu_mode_nu_1pi0_Ppi0_cov   	            = "nuppi0xsecerrormatrix.txt";			  
  fnu_mode_nu_1pi0_CosThetapi0_cov          = "nucosthetapi0xsecerrormatrix.txt";			 
  fnu_mode_nu_nubar_1pi0_Ppi0_cov           = "combinedsignnumodeppi0xsecerrormatrix.txt";	   
  fnu_mode_nu_nubar_1pi0_CosThetapi0_cov    = "combinedsignnumodecosthetapi0xsecerrormatrix.txt";	   
  fnubar_mode_nu_1pi0_Ppi0_cov 	            = "nubarppi0xsecerrormatrix.txt";			  
  fnubar_mode_nu_1pi0_CosThetapi0_cov       = "nubarcosthetapi0xsecerrormatrix.txt";		   
  fnubar_mode_nu_nubar_1pi0_Ppi0_cov        = "combinedsignnubarmodeppi0xsecerrormatrix.txt";	  
  fnubar_mode_nu_nubar_1pi0_CosThetapi0_cov = "combinedsignnubarmodecosthetapi0xsecerrormatrix.txt";
  
}

//____________________________________________________________________________
MBNC1pi0Data::~MBNC1pi0Data()
{

}
//____________________________________________________________________________
bool MBNC1pi0Data::Read()
{
  // Read xsec data
  ReadtoArray( fnu_mode_nu_1pi0_Ppi0_xsec                , fNuModeNuPpi0Xsec               );
  ReadtoArray( fnu_mode_nu_1pi0_CosThetapi0_xsec         , fNuModeNuCosThetapi0Xsec        );
  ReadtoArray( fnu_mode_nu_nubar_1pi0_Ppi0_xsec          , fNuModeNuNuBarPpi0Xsec          );
  ReadtoArray( fnu_mode_nu_nubar_1pi0_CosThetapi0_xsec   , fNuModeNuNuBarCosThetapi0Xsec   );
  ReadtoArray( fnubar_mode_nu_1pi0_Ppi0_xsec             , fNuBarModePpi0Xsec              );
  ReadtoArray( fnubar_mode_nu_1pi0_CosThetapi0_xsec      , fNuBarModeCosThetapi0Xsec       );
  ReadtoArray( fnubar_mode_nu_nubar_1pi0_Ppi0_xsec       , fNuBarModeNuNuBarPpi0Xsec       );
  ReadtoArray( fnubar_mode_nu_nubar_1pi0_CosThetapi0_xsec, fNuBarModeNuNubarCosThetapi0Xsec);

  // Read error matrices (changed from read errors, now have a 1d array)
  ReadtoArray( fnu_mode_nu_1pi0_Ppi0_cov                , fNuModeNuPpi0Cov                );
  ReadtoArray( fnu_mode_nu_1pi0_CosThetapi0_cov         , fNuModeNuCosThetapi0Cov         );
  ReadtoArray( fnu_mode_nu_nubar_1pi0_Ppi0_cov          , fNuModeNuNuBarPpi0Cov           );
  ReadtoArray( fnu_mode_nu_nubar_1pi0_CosThetapi0_cov   , fNuModeNuNuBarCosThetapi0Cov    );
  ReadtoArray( fnubar_mode_nu_1pi0_Ppi0_cov             , fNuBarModePpi0Cov               );
  ReadtoArray( fnubar_mode_nu_1pi0_CosThetapi0_cov      , fNuBarModeCosThetapi0Cov        );
  ReadtoArray( fnubar_mode_nu_nubar_1pi0_Ppi0_cov       , fNuBarModeNuNuBarPpi0Cov        );
  ReadtoArray( fnubar_mode_nu_nubar_1pi0_CosThetapi0_cov, fNuBarModeNuNubarCosThetapi0Cov );
  return true;
}
//____________________________________________________________________________
bool MBNC1pi0Data::ReadtoArray(std::string data_bins_file_string, double * data_array)
{
  int i=0;

  const char * data_bins_file = data_bins_file_string.c_str();
  
  std::ifstream BinsFile(data_bins_file);
  if (BinsFile.is_open()){
    
    if (BinsFile.good()){
      std::string fileLine;
      double kBinEdge(0);
      while (getline(BinsFile,fileLine)){

	std::stringstream fStrStream(fileLine);     
	// Read into binning array
	// All bins are on a single line
	while (fStrStream >> kBinEdge) {
          data_array[i++] = kBinEdge;	 
	}
      }
    }
  } else {
    return false;
  }
  return true;
}
//____________________________________________________________________________
bool MBNC1pi0Data::ReadErrors(std::string data_error_file_string, double * data_error_array)
{
  return true;
}

//____________________________________________________________________________
void MBNC1pi0Data::Write(const char * output_file)
{
  /*
  // Declare ROOT histogram for every CosTheta slice
  TH1D * histo[fNCosThetamuBins];
  for(int i = 0; i < fNCosThetamuBins; i++) {
    const char * title = "";
    const char * name_data = Form("data_costhetabin_%d",i);
    histo[i] = new TH1D(name_data,title,fNTmuBins,fTmuBinEdges);
  }

  // Fill root histogram with the data  
  for(int i = 0; i < fNCosThetamuBins; i++) {
    for(int j = 0; j < fNTmuBins; j++) {
      double xsec = fdDiffXSecArray[i][j];
      histo[i]->SetBinContent(j+1,xsec);
    }
  }

  // Use ROOT TFile Class to write data
  TFile f(output_file,"recreate");
  for(int i = 0; i < fNCosThetamuBins; i++) {
    histo[i]->Write();
  }
  f.Close();
  */
}

//____________________________________________________________________________
void MBNC1pi0Data::ReplaceStringInPlace(string& subject, const string& search, const string& replace)
{
  size_t pos = 0;
  // Loop while replacement in string is not at end of string 
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    // move to end of replaced string
    pos += replace.length();
  }
}
//____________________________________________________________________________
bool MBNC1pi0Data::PrintArray(double* data_array, int array_size)
{
  return true;
}
//____________________________________________________________________________
bool  MBNC1pi0Data::Print2DArray(double* data_array, int array_size_x, int array_size_y)
{
  return true;
}
//____________________________________________________________________________
void MBNC1pi0Data::Print()
{
  
}
//____________________________________________________________________________
bool  MBNC1pi0Data::InitialiseArray(double* data_array, int array_size)
{
  for (int i = 0; i < array_size; i++) {
    data_array[i] = 0;
  }
  return true;
}
//____________________________________________________________________________
