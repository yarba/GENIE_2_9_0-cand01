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

#include "Utils/SystemUtils.h"

#include <TSystem.h>

#include "DataComp/MiniBooNE/MBCC1pipBinning.h"

using namespace genie;
using namespace genie::mc_vs_data;
using namespace genie::mc_vs_data::constants::MB;

void ReadLineToArray(double * array0, double d0, int & i);
void PrintArrays(double * array0, int dim);
void ReadMtx(std::string fileLine, int & Nrow, int & Ncol, double * xBins, double * yBins, int & i);
//____________________________________________________________________________
MBCC1pipBinning::MBCC1pipBinning()
{
  std::string basedir = string( gSystem->Getenv("GENIE") );

  // initialise arrays holding binning information
  fCC1pipData_file = basedir + "/data/validation/vA/xsec/differential/miniboone/ccpip/ccpipXSecTables.txt"; 
  
  Initialise( fEnuBinEdges, CC1pip::kNEnuBins  + 1, CC1pip::kNEnuLastBin );              
  Initialise( fTpiBinEdges, CC1pip::kNTpiBins  + 1, CC1pip::kNTpiLastBin );              
  Initialise( fQ2BinEdges,  CC1pip::kNQ2Bins   + 1, CC1pip::kNQ2LastBin  );               
  Initialise( fTmuBinEdges, CC1pip::kNTmuBins  + 1, CC1pip::kNTmuLastBin );               
    
  Initialise( fDiffQ2EnuQ2BinEdges,   CC1pip::kNDiffQ2EnuQ2Bins   + 1, CC1pip::kNDiffQ2EnuQ2LastBin  );      
  Initialise( fDiffQ2EnuBinEdges,     CC1pip::kNDiffQ2EnuBins     + 1, CC1pip::kNDiffQ2EnuLastBin    );        
  Initialise( fDiffTmuEnuTmuBinEdges, CC1pip::kNDiffTmuEnuTmuBins + 1, CC1pip::kNDiffTmuEnuTmuLastBin);    
  Initialise( fDiffTmuEnuBinEdges,    CC1pip::kNDiffTmuEnuBins    + 1, CC1pip::kNDiffTmuEnuLastBin   );       
  Initialise( fDiffTpiEnuTpiBinEdges, CC1pip::kNDiffTpiEnuTpiBins + 1, CC1pip::kNDiffTpiEnuTpiLastBin);    
  Initialise( fDiffTpiEnuBinEdges,    CC1pip::kNDiffTpiEnuBins    + 1, CC1pip::kNDiffTpiEnuLastBin   );       
    	      
  Initialise( fDbldiffCosThetamuBinEdges, CC1pip::kNDbldiffCosThetamuBins  + 1, CC1pip::kNDbldiffCosThetamuLastBin);
  Initialise( fDbldiffTmuBinEdges,        CC1pip::kNDbldiffTmuBins         + 1, CC1pip::kNDbldiffTmuLastBin       );       
  Initialise( fDbldiffCosThetapiBinEdges, CC1pip::kNDbldiffCosThetapiBins  + 1, CC1pip::kNDbldiffCosThetapiLastBin);
  Initialise( fDbldiffTpiBinEdges,        CC1pip::kNDbldiffTpiBins         + 1, CC1pip::kNDbldiffTpiLastBin       );
         
}
//____________________________________________________________________________
MBCC1pipBinning::~MBCC1pipBinning()
{

}
//____________________________________________________________________________
bool MBCC1pipBinning::Read()
{
  // One text file contains all the data, in this class bins only are read
  const char * data_file = fCC1pipData_file.c_str(); 
  int i = 0, counter = 0, blanklines = 0, Ncol = 0, Nrow = 0, mtxctr = 0, mtxidx = 0;
      
  std::ifstream infile(data_file);
  
  if (infile.is_open()){
    if (infile.good()){
      
      std::string find = "|", replace = "  ", spaces = "     ", zero = "0.0";
      std::string Bin = "Bin";
      std::string fileLine;
      double kBins(0), kXSec(0), kXSecError(0);
      std::string plusminus, delim;

      while ( getline( infile, fileLine ) ){

	// Keep track of blank lines to parse file
	if ( fileLine.empty() ){
	  blanklines++;
	  continue;
	} else if ( fileLine.find("The", 0) != std::string::npos ) {
	  continue;
	} else if ( fileLine.find("Bin (", 0) != std::string::npos ) {
	  counter++;
	  continue;
	} else if ( blanklines <= 2 && counter <= 4 ) {
	  blanklines = 0;
       
	  std::stringstream fStrStream(fileLine);
	  while (fStrStream >> kBins >> delim >> kXSec >> plusminus >> kXSecError) {

	    if (counter == 1){
	      fEnuBinEdges[i++] = kBins;
	      if ( i == CC1pip::kNEnuBins ) {
		i = 0;
	      }
	    } else if (counter == 2) {
	      fTpiBinEdges[i++] = kBins;
	      if (i == CC1pip::kNTpiBins){
		i = 0;
	      }
	    } else if (counter == 3) {
	      fQ2BinEdges[i++] = kBins;
	      if ( i == CC1pip::kNQ2Bins ){
		i = 0;
	      }
	    } else if (counter == 4) {
	      fTmuBinEdges[i++] = kBins;
	      if ( i == CC1pip::kNTmuBins){
		i = 0;
		// Increment counter so that matrices are read in next
		counter++;
	      }
	    }
	  }

	} else if ( counter >= 4  || blanklines >= 3 ) {
	  // blanklines between matrices >= 3
	  if ( blanklines > 3 ) {
	    mtxctr++;
	    mtxidx = 0;
	    //std::cout << "Matrix #: " << mtxctr << std::endl;
	    Nrow = 0;
	  }
	  
	  blanklines = 0;
	 
	  // Replace strings in line so that input can be parsed using string stream 
	  ReplaceStringInPlace(fileLine, Bin, zero);
	  ReplaceStringInPlace(fileLine, spaces, zero);
	  ReplaceStringInPlace(fileLine, find, replace);
	  

	  // Even values of mtxctr read error matrices
    	  if      (mtxctr == 1) { ReadMtx(fileLine, Nrow, Ncol, fDiffQ2EnuQ2BinEdges, fDiffQ2EnuBinEdges, mtxidx);  }
	  //   	  else if (mtxctr == 2) { ReadMtx(fileLine, Nrow, Ncol, kXSecErrorM1, mtxidx); 	  }
   	  else if (mtxctr == 3) { ReadMtx(fileLine, Nrow, Ncol, fDiffTmuEnuTmuBinEdges, fDiffTmuEnuBinEdges, mtxidx);  }
	  //   	  else if (mtxctr == 4) { ReadMtx(fileLine, Nrow, Ncol, kXSecErrorM2, mtxidx);  	  }
   	  else if (mtxctr == 5) { ReadMtx(fileLine, Nrow, Ncol, fDiffTpiEnuTpiBinEdges, fDiffTpiEnuBinEdges, mtxidx);  }
	  //   	  else if (mtxctr == 6) { ReadMtx(fileLine, Nrow, Ncol, kXSecErrorM3, mtxidx);     }
   	  else if (mtxctr == 7) { ReadMtx(fileLine, Nrow, Ncol, fDbldiffCosThetamuBinEdges, fDbldiffTmuBinEdges, mtxidx);  }
	  //   	  else if (mtxctr == 8) { ReadMtx(fileLine, Nrow, Ncol, kXSecErrorM4, mtxidx); 	  }
   	  else if (mtxctr == 9) { ReadMtx(fileLine, Nrow, Ncol, fDbldiffCosThetapiBinEdges, fDbldiffTpiBinEdges, mtxidx);  }
	  //   	  else if (mtxctr == 10){ ReadMtx(fileLine, Nrow, Ncol, kXSecErrorM5, mtxidx); 	  }
   	}
      }
    }
  }

  //  ROOT histogram requires bins in ascending order
  //  Arrays are of size CC1pip::constant + 1, the last value, added in by Initialise is in the correct position 
  std::reverse( fDiffQ2EnuQ2BinEdges,   fDiffQ2EnuQ2BinEdges   + CC1pip::kNDiffQ2EnuQ2Bins   );      
  std::reverse( fDiffTmuEnuTmuBinEdges, fDiffTmuEnuTmuBinEdges + CC1pip::kNDiffTmuEnuTmuBins );    
  std::reverse( fDiffTpiEnuTpiBinEdges, fDiffTpiEnuTpiBinEdges + CC1pip::kNDiffTpiEnuTpiBins );    

  std::reverse( fDbldiffCosThetamuBinEdges, fDbldiffCosThetamuBinEdges + CC1pip::kNDbldiffCosThetamuBins );
  std::reverse( fDbldiffCosThetapiBinEdges, fDbldiffCosThetapiBinEdges + CC1pip::kNDbldiffCosThetapiBins );

  return true;
}
//____________________________________________________________________________
bool MBCC1pipBinning::ReadArray(std::string BinEdgesFile, double * data_array)
{
  return true;
}
//____________________________________________________________________________
bool MBCC1pipBinning::Initialise(double * binedges_data_array, int data_dims, double upperbinedge)
{
  for (int i = 0; i < (data_dims - 1); i++) {
    binedges_data_array[i] = 0;
  }
  // Final upper bin edge not included in text data file
  binedges_data_array[data_dims - 1] = upperbinedge;
  return true;
}
//____________________________________________________________________________
void MBCC1pipBinning::ReplaceStringInPlace(std::string& subject, std::string& search, std::string& replace)
{
  size_t pos = 0;
  // Loop while search in string is not at end of string 
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    // move to end of replaced string
    pos += replace.length();
  }
}

//____________________________________________________________________________
void PrintArrays(double * array0, double * array1, double * array2, int dim)
{
  std::cout << "Printing Array" << std::endl;
  
  for (int i=0; i < dim; i++){
    std::cout << std::setw(6) << array0[i] 
	      << std::endl;
  }    
}  
//____________________________________________________________________________
void PrintArrays(double * array0, int dim)
{
  std::cout << "Printing Array" << std::endl;
  
  for (int i=0; i < dim; i++){
    std::cout << std::setw(6) << array0[i] 
	      << std::endl;
  }    
} 
//____________________________________________________________________________
void ReadMtx(std::string fileLine, int & Nrow, int & Ncol, double * xBins, double * yBins, int & i)
{
  std::stringstream fStrStream(fileLine);
  double kBins(0);
  while (fStrStream >> kBins){
    // Replaced instances of "Bin" with zero 
    if ( Ncol == 0 ) {
      if ( Nrow == 0 ) {
	Ncol++;
	continue;		
      }
      // y bins, start on second row, index at zero 
      yBins[Nrow-1] = kBins;

    } else if ( Nrow == 0 ) {
      // x bins
      xBins[Ncol-1] = kBins;

    } else {
      // xyBins[i++] = kBins;
    }
    Ncol++;
  }
  
  Nrow++;
  Ncol = 0;
} 
