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

#include <TSystem.h>

#include "DataComp/MiniBooNE/MBCCQEBinning.h"

using namespace genie;
using namespace genie::mc_vs_data;
using namespace genie::mc_vs_data::constants::MB;

//____________________________________________________________________________
MBCCQEBinning::MBCCQEBinning(void)
{
  fBinningFilename = "";

  // initialise arrays holding binning information
  for (int i=0; i < CCQE::kNCosThetamuBins +1; i++) {
    fCosThetamuBinEdges[i] = 0;
  }
  for (int i = 0; i < CCQE::kNTmuBins + 1; i++) {
    fTmuBinEdges[i] = 0;
  }
}
//____________________________________________________________________________
MBCCQEBinning::~MBCCQEBinning()
{

}
//____________________________________________________________________________
bool MBCCQEBinning::Read(string binning_filename)
{
  fBinningFilename = binning_filename;

  int idx = 0; 

  std::ifstream NumuBinsFile(fBinningFilename.c_str());

  if (NumuBinsFile.is_open()){
    if (NumuBinsFile.good()){     
      double kBinEdge(0);  
      std::string fileLine;
      bool kswitch = false;
      
      NumuBinsFile.seekg(0);
      while (std::getline(NumuBinsFile,fileLine)){
	
	std::istringstream StrStreamLine(fileLine);
	
	// Read into Tmu bins till blank line (switch == true) 
	// then read into Cos bins. 
	if (fileLine != "" && kswitch == false){
	  StrStreamLine >> kBinEdge;
	  fTmuBinEdges[idx++] = kBinEdge;
	} 
	else if (fileLine == "") {
	  kswitch = true;
          idx=0;	
	} 
	else if (fileLine != "" && kswitch == true){
	  StrStreamLine >> kBinEdge;
	  fCosThetamuBinEdges[idx++] = kBinEdge;
	} else {
	  std::cout << "Cannot read file line" << std::endl;
	  return false;
	}
      }
      } else {
       	return false;
      }
  }
  // ROOT histogram requires bins in increasing order
  std::reverse(fCosThetamuBinEdges, fCosThetamuBinEdges + CCQE::kNCosThetamuBins + 1);
  return true;
}
//____________________________________________________________________________
