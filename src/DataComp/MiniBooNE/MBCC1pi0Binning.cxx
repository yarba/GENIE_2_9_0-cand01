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

#include "DataComp/MiniBooNE/MBCC1pi0Binning.h"

using namespace genie;
using namespace genie::mc_vs_data;

using namespace genie::mc_vs_data::constants::MB;

//____________________________________________________________________________
MBCC1pi0Binning::MBCC1pi0Binning()
{
  //  std::string fCC1pi0Path = "/data/validation/vA/xsec/differential/numu_ccpi0"; 
  //  std::string GeniePath = std::getenv ("GENIE");
  //  std::string CC1pi0Path = GeniePath + fCC1pi0Path;
  std::string CC1pi0Path = "ccpi0/";

  fEnuBinEdges_file          = CC1pi0Path + "totalxsec.txt";
  fQ2BinEdges_file           = CC1pi0Path + "dxsecdq2.txt";
  fEmuBinEdges_file          = CC1pi0Path + "dxsecdemu.txt";
  fCosThetamuBinEdges_file   = CC1pi0Path + "dxsecdcosmu.txt";
  fPpi0BinEdges_file         = CC1pi0Path + "dxsecdppi.txt";
  fCosThetapi0BinEdges_file  = CC1pi0Path + "dxsecdcospi.txt";

  // initialise arrays holding binning information
  
  Initialise( fEnuBinEdges        , CC1pi0::kNEnuBins         );
  Initialise( fQ2BinEdges         , CC1pi0::kNQ2Bins          );
  Initialise( fEmuBinEdges        , CC1pi0::kNEmuBins         );
  Initialise( fCosThetamuBinEdges , CC1pi0::kNCosThetamuBins  );
  Initialise( fPpi0BinEdges       , CC1pi0::kNPpi0Bins        );
  Initialise( fCosThetapi0BinEdges, CC1pi0::kNCosThetapi0Bins );

}
//____________________________________________________________________________
MBCC1pi0Binning::~MBCC1pi0Binning()
{

}
//____________________________________________________________________________
bool MBCC1pi0Binning::ReadArray(std::string BinEdgesFile, double * data_array)
{
  // Files have same structure
  // Read line which says Bin Boundaries, then skip next blank line 
  // Read bin boundaries 
  
  std::ifstream dDiffXSecFile(BinEdgesFile.c_str());
  
  if (dDiffXSecFile.is_open()){
    
    if (dDiffXSecFile.good()){
      std::string fileLine;
      double kdDiffXSec(0);
      int counter = 0 , blanklines = 0, i = 0 ;
      while ( getline( dDiffXSecFile, fileLine ) ){
	// if file has char "Bin" on line then increment counter
	if ( fileLine.find("Bin", 0 ) != std::string::npos ) {
	  counter++;
	}
	if ( fileLine.empty() ){
	  blanklines++;
	} else {
	  blanklines = 0;
	}  
	if ( counter == 1 ) {
	  std::stringstream fStrStream(fileLine);
	  while (fStrStream >> kdDiffXSec) {
	    //Read into array
	    data_array[i++] = kdDiffXSec;
	  }
	}
	// stop reading file after two blank lines 
	if (blanklines == 2 && counter == 1) break;
      }
      
    }
    
  }
  return true;
}
//____________________________________________________________________________
bool MBCC1pi0Binning::Read()
{

  ReadArray( fEnuBinEdges_file        , fEnuBinEdges         );
  ReadArray( fQ2BinEdges_file         , fQ2BinEdges          );
  ReadArray( fEmuBinEdges_file        , fEmuBinEdges         );
  ReadArray( fCosThetamuBinEdges_file , fCosThetamuBinEdges  );
  ReadArray( fPpi0BinEdges_file       , fPpi0BinEdges        );
  ReadArray( fCosThetapi0BinEdges_file, fCosThetapi0BinEdges );

  return true;
}
//____________________________________________________________________________
bool MBCC1pi0Binning::Initialise(double * data_array, int data_dims)
{
  for (int i = 0; i < data_dims ; i++){
    data_array[i] = 0;
  }
  return true;
}
