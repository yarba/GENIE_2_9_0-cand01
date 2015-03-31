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
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

#include "DataComp/MiniBooNE/MBNC1pi0Binning.h"
#include "DataComp/MiniBooNE/MBConstants.h"

using namespace genie;
using namespace genie::mc_vs_data;
using namespace genie::mc_vs_data::constants::MB;

//____________________________________________________________________________
MBNC1pi0Binning::MBNC1pi0Binning()
{
  // Set paths to binning data
  // Data should be in Genie data section

//  std::string fNC1pi0Path = "/data/validation/vA/xsec/differential/numu_ncpi0"; 
//  std::string GeniePath = std::getenv ("GENIE");
//  std::string NC1pi0Path = GeniePath + fNC1pi0Path;
  std::string NC1pi0Path = "ncpi0/";
  
  fnu_mode_nu_1pi0_Ppi0                 = NC1pi0Path + "nuppi0binedges.txt"; 			    
  fnu_mode_nu_1pi0_CosThetapi0          = NC1pi0Path + "nucosthetapi0binedges.txt";		     
  fnu_mode_nu_nubar_1pi0_Ppi0           = NC1pi0Path + "combinedsignnumodeppi0binedges.txt";	  
  fnu_mode_nu_nubar_1pi0_CosThetapi0    = NC1pi0Path + "combinedsignnumodecosthetapi0binedges.txt";   
  fnubar_mode_nu_1pi0_Ppi0              = NC1pi0Path + "nubarppi0binedges.txt";		  
  fnubar_mode_nu_1pi0_CosThetapi0       = NC1pi0Path + "nubarcosthetapi0binedges.txt";		 
  fnubar_mode_nu_nubar_1pi0_Ppi0        = NC1pi0Path + "combinedsignnubarmodeppi0binedges.txt"; 	  
  fnubar_mode_nu_nubar_1pi0_CosThetapi0 = NC1pi0Path + "combinedsignnubarmodecosthetapi0binedges.txt";
    
  // Initialise arrays holding binning information 
  InitialiseArray( fNuModeNuPpi0BinEdges,                NC1pi0::kNNuModeNuPpi0Bins                );
  InitialiseArray( fNuModeNuCosThetapi0BinEdges,         NC1pi0::kNNuModeNuCosThetapi0Bins         );
  InitialiseArray( fNuModeNuNuBarPpi0BinEdges,           NC1pi0::kNNuModeNuNuBarPpi0Bins           );
  InitialiseArray( fNuModeNuNuBarCosThetapi0BinEdges,    NC1pi0::kNNuModeNuNuBarCosThetapi0Bins    );
  InitialiseArray( fNuBarModePpi0BinEdges,               NC1pi0::kNNuBarModeNuPpi0Bins             );
  InitialiseArray( fNuBarModeCosThetapi0BinEdges,        NC1pi0::kNNuBarModeNuCosThetapi0Bins      );
  InitialiseArray( fNuBarModeNuNuBarPpi0BinEdges,        NC1pi0::kNNuBarModeNuNuBarPpi0Bins        );
  InitialiseArray( fNuBarModeNuNubarCosThetapi0BinEdges, NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins );
}
//____________________________________________________________________________
MBNC1pi0Binning::~MBNC1pi0Binning()
{

}
//____________________________________________________________________________
bool MBNC1pi0Binning::Read()
{
  ReadtoArray( fnu_mode_nu_1pi0_Ppi0,    	      fNuModeNuPpi0BinEdges                );  
  ReadtoArray( fnu_mode_nu_1pi0_CosThetapi0,          fNuModeNuCosThetapi0BinEdges         );  
  ReadtoArray( fnu_mode_nu_nubar_1pi0_Ppi0,           fNuModeNuNuBarPpi0BinEdges           );  
  ReadtoArray( fnu_mode_nu_nubar_1pi0_CosThetapi0,    fNuModeNuNuBarCosThetapi0BinEdges    );  
  ReadtoArray( fnubar_mode_nu_1pi0_Ppi0,  	      fNuBarModePpi0BinEdges               );   
  ReadtoArray( fnubar_mode_nu_1pi0_CosThetapi0,       fNuBarModeCosThetapi0BinEdges        );    
  ReadtoArray( fnubar_mode_nu_nubar_1pi0_Ppi0,        fNuBarModeNuNuBarPpi0BinEdges        );    
  ReadtoArray( fnubar_mode_nu_nubar_1pi0_CosThetapi0, fNuBarModeNuNubarCosThetapi0BinEdges ); 
  
  return true;
}
//____________________________________________________________________________
bool MBNC1pi0Binning::Print()
{
  PrintArray( fNuModeNuPpi0BinEdges,                NC1pi0::kNNuModeNuPpi0Bins                );
  PrintArray( fNuModeNuCosThetapi0BinEdges,         NC1pi0::kNNuModeNuCosThetapi0Bins         );
  PrintArray( fNuModeNuNuBarPpi0BinEdges,           NC1pi0::kNNuModeNuNuBarPpi0Bins           );
  PrintArray( fNuModeNuNuBarCosThetapi0BinEdges,    NC1pi0::kNNuModeNuNuBarCosThetapi0Bins    );
  PrintArray( fNuBarModePpi0BinEdges,               NC1pi0::kNNuBarModeNuPpi0Bins             );
  PrintArray( fNuBarModeCosThetapi0BinEdges,        NC1pi0::kNNuBarModeNuCosThetapi0Bins      );
  PrintArray( fNuBarModeNuNuBarPpi0BinEdges,        NC1pi0::kNNuBarModeNuNuBarPpi0Bins        );
  PrintArray( fNuBarModeNuNubarCosThetapi0BinEdges, NC1pi0::kNNuBarModeNuNuBarCosThetapi0Bins );

  return true;
  }
//____________________________________________________________________________
bool MBNC1pi0Binning::ReadtoArray(std::string data_bins_file_string, double * data_array)
{
  int i=0;

  // std::cout << "File Path: " << data_bins_file_string << "\n";
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
bool MBNC1pi0Binning::PrintArray(double* data_array, int array_size)
{
  for (int i  = 0; i < array_size + 1; i++) {
      std::cout << std::setw(9) << data_array[i] ; 
    }
  std::cout << std::endl;
  return true;
}
//____________________________________________________________________________
bool MBNC1pi0Binning::InitialiseArray(double* data_array, int array_size)
{
  for (int i  = 0; i < array_size + 1; i++) {
    data_array[i] = -999 ; 
    }
  return true;
}
//____________________________________________________________________________
