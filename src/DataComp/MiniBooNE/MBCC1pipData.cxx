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
#include <TFile.h>
#include <TTree.h>
#include <TIterator.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>

#include "Messenger/Messenger.h"
#include "DataComp/MiniBooNE/MBCC1pipData.h"
#include "DataComp/MiniBooNE/MBCC1pipBinning.h"

using namespace genie;
using namespace genie::mc_vs_data;
using namespace genie::mc_vs_data::constants::MB;

void WriteMtx(std::string fileLine, int & Nrow, int & Ncol, double * xyBins, int & i);
void ReadLineToArray(double * XSecArray, double XSecVal, double * UncertArray,  double UncertVal, int & i);
//____________________________________________________________________________
MBCC1pipData::MBCC1pipData(MBCC1pipBinning & binning)
{
 
  fBinning = new MBCC1pipBinning;

  fCC1pipPath = ""; 
  fCC1pipData_file = "";


  // Initialise arrays holding XSec information 
  Initialise( fEnuXSec , CC1pip::kNEnuBins );
  Initialise( fTpiXSec , CC1pip::kNTpiBins );
  Initialise( fQ2XSec  , CC1pip::kNQ2Bins  );
  Initialise( fTmuXSec , CC1pip::kNTmuBins );

  Initialise( fDiffQ2EnuXSec       , ( CC1pip::kNDiffQ2EnuBins     * CC1pip::kNDiffQ2EnuQ2Bins    )   );
  Initialise( fDiffTmuEnuXSec      , ( CC1pip::kNDiffTmuEnuTmuBins * CC1pip::kNDiffTmuEnuBins     )   );
  Initialise( fDiffTpiEnuXSec      , ( CC1pip::kNDiffTpiEnuTpiBins * CC1pip::kNDiffTpiEnuBins     )   ); 
  Initialise( fDbldiffTmuCosmuXSec , ( CC1pip::kNDbldiffCosThetamuBins * CC1pip::kNDbldiffTmuBins )   );
  Initialise( fDbldiffTpiCospiXSec , ( CC1pip::kNDbldiffCosThetapiBins * CC1pip::kNDbldiffTpiBins )   );
  
  // Initialise arrays holding uncertainty information
  Initialise( fEnuXSecUncert , CC1pip::kNEnuBins );
  Initialise( fTpiXSecUncert , CC1pip::kNTpiBins );
  Initialise( fQ2XSecUncert  , CC1pip::kNQ2Bins  );
  Initialise( fTmuXSecUncert , CC1pip::kNTmuBins );

  Initialise( fDiffQ2EnuXSecUncert       , ( CC1pip::kNDiffQ2EnuBins         * CC1pip::kNDiffQ2EnuQ2Bins )   );
  Initialise( fDiffTmuEnuXSecUncert      , ( CC1pip::kNDiffTmuEnuTmuBins     * CC1pip::kNDiffTmuEnuBins  )   );
  Initialise( fDiffTpiEnuXSecUncert      , ( CC1pip::kNDiffTpiEnuTpiBins     * CC1pip::kNDiffTpiEnuBins  )   );
  Initialise( fDbldiffTmuCosmuXSecUncert , ( CC1pip::kNDbldiffCosThetamuBins * CC1pip::kNDbldiffTmuBins  )   );
  Initialise( fDbldiffTpiCospiXSecUncert , ( CC1pip::kNDbldiffCosThetapiBins * CC1pip::kNDbldiffTpiBins  )   );


  // Get binning data pointers
  fEnuBinEdges = binning.GetEnuBinEdges();              
  fTpiBinEdges = binning.GetTpiBinEdges();              
  fQ2BinEdges  = binning.GetQ2BinEdges() ;              
  fTmuBinEdges = binning.GetTmuBinEdges();              
  					                                 
  fDiffQ2EnuQ2BinEdges      = binning.GetDiffQ2EnuQ2BinEdges()       ;   
  fDiffQ2EnuBinEdges        = binning.GetDiffQ2EnuBinEdges()         ;
  fDiffTmuEnuTmuBinEdges    = binning.GetDiffTmuEnuTmuBinEdges()     ;
  fDiffTmuEnuBinEdges       = binning.GetDiffTmuEnuBinEdges()        ;
  fDiffTpiEnuTpiBinEdges    = binning.GetDiffTpiEnuTpiBinEdges()     ;
  fDiffTpiEnuBinEdges       = binning.GetDiffTpiEnuBinEdges()        ;
  			     	                                      
  fDbldiffCosThetamuBinEdge = binning.GetDbldiffCosThetamuBinEdges() ;
  fDbldiffTmuBinEdges       = binning.GetDbldiffTmuBinEdges()        ;
  fDbldiffCosThetapiBinEdge = binning.GetDbldiffCosThetapiBinEdges() ;
  fDbldiffTpiBinEdges       = binning.GetDbldiffTpiBinEdges()        ;


}
//____________________________________________________________________________
MBCC1pipData::~MBCC1pipData()
{
  std::cout << "At class destructor: Data" << std::endl;
  
  delete fBinning;

}
//____________________________________________________________________________
bool MBCC1pipData::Write(
  TObjArray * array, const char * tag, Option_t * write_opt)
{
  LOG("MiniBooNE", pDEBUG) << "Writing results to input object array";

  TH1D * hPi[CC1pip::kNDbldiffCosThetapiBins];

  for(int i = 0; i < CC1pip::kNDbldiffCosThetapiBins; i++) {
    const char * title = "";
    const char * name  = Form("%s_%s_d2sig_costhetabin_%d",tag,this->Name().c_str(),i);
    hPi[i] = new TH1D(name,title,CC1pip::kNDbldiffTpiBins,fDbldiffTpiBinEdges);
  }
  for(int i = 0; i < CC1pip::kNDbldiffCosThetapiBins; i++) {
    for(int j = 0; j < CC1pip::kNDbldiffTpiBins; j++) {
      double xsec =  fDbldiffTpiCospiXSec[ CC1pip::kNDbldiffCosThetapiBins*i + j ]; // 1D array
      hPi[i]->SetBinContent(j+1,xsec);
    }
  }
  for(int i = 0; i < CC1pip::kNDbldiffCosThetapiBins; i++) {
    array->Add(hPi[i]); // transfers ownership?
  }

  
  TH1D * hMu[CC1pip::kNDbldiffCosThetamuBins];
 
  for(int i = 0; i < CC1pip::kNDbldiffCosThetamuBins; i++) {
    const char * title = "";
    const char * name  = Form("%s_%s_d2sig_costhetabin_%d",tag,this->Name().c_str(),i);
    hMu[i] = new TH1D(name,title,CC1pip::kNDbldiffTpiBins,fDbldiffTmuBinEdges);
  }
  for(int i = 0; i < CC1pip::kNDbldiffCosThetamuBins; i++) {
    for(int j = 0; j < CC1pip::kNDbldiffTpiBins; j++) {
      double xsec = fDbldiffTmuCosmuXSec[ CC1pip::kNDbldiffCosThetamuBins*i + j ]; // 1D array
      hMu[i]->SetBinContent(j+1,xsec);
    }
  }
  for(int i = 0; i < CC1pip::kNDbldiffCosThetamuBins; i++) {
    array->Add(hMu[i]); // transfers ownership?
  }



  // Need histogram for each separate data class 

  // Write data to histogram

  // Write histograms to file
  return true;
}
//____________________________________________________________________________
bool MBCC1pipData::Read(string file, Option_t * opt)
{
  LOG("MiniBooNE", pDEBUG) 
     << "Reading MiniBooNE double-differential CC 1 pi+ cross-sections";

  // ----------------------------------- needs improvement / start
  // Hardcoded for now --
  // Get all that stuff from the input XML file
  //

  string basedir = string( gSystem->Getenv("GENIE") );
  

  // All experimental data releases in one file
  
  string fCC1pipPath = "/data/validation/vA/xsec/differential/miniboone/ccpip/ccpipXSecTables.txt";
                         
  fCC1pipData_file   = basedir + fCC1pipPath;

  // ----------------------------------- needs improvement / end

  // Read binning
  fBinning->Read();
 
  // Read central values and fractional errors   
  LOG("MiniBooNE", pDEBUG) 
     << "Reading central values from: " << fCC1pipData_file;

  this->ReadDataFile(fCC1pipData_file);

  return true;
}
//____________________________________________________________________________
bool MBCC1pipData::ReadDataFile(string data_file_str)
{
  const char * data_file = data_file_str.c_str(); 
  int i = 0, counter = 0, blanklines = 0, Ncol = 0, Nrow = 0, mtxctr = 0, mtxidx = 0;

  std::ifstream infile(data_file);
  
  if ( infile.is_open() ){
    if ( infile.good() ){
      
      std::string find = "|", replace = "  ", spaces = "     ", zero = "0.0";
      std::string Bin = "Bin";
      std::string fileLine;
      double kBins(0), kXSec(0), kXSecError(0);
      std::string plusminus, delim;

      while ( getline(infile, fileLine) ){

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
	      ReadLineToArray(fEnuXSec, kXSec, fEnuXSecUncert, kXSecError, i);
	      if ( i == CC1pip::kNEnuBins ) {
		i = 0;
	      }
	    } else if (counter == 2) {
	      ReadLineToArray(fTpiXSec, kXSec, fTpiXSecUncert, kXSecError, i);
	      if (i == CC1pip::kNTpiBins){
		i = 0;
	      }
	    } else if (counter == 3) {
	      ReadLineToArray(fQ2XSec, kXSec, fQ2XSecUncert, kXSecError, i);
	      if ( i == CC1pip::kNQ2Bins ){
		i = 0;
	      }
	    } else if (counter == 4) {
	      ReadLineToArray(fTmuXSec, kXSec, fTmuXSecUncert, kXSecError, i);
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
    	  if       (mtxctr == 1) { WriteMtx(fileLine, Nrow, Ncol, fDiffQ2EnuXSec,	      mtxidx); }
	  else	if (mtxctr == 2) { WriteMtx(fileLine, Nrow, Ncol, fDiffQ2EnuXSecUncert ,      mtxidx); }
   	  else	if (mtxctr == 3) { WriteMtx(fileLine, Nrow, Ncol, fDiffTmuEnuXSec,	      mtxidx); }
	  else	if (mtxctr == 4) { WriteMtx(fileLine, Nrow, Ncol, fDiffTmuEnuXSecUncert,      mtxidx); }
   	  else	if (mtxctr == 5) { WriteMtx(fileLine, Nrow, Ncol, fDiffTpiEnuXSec ,	      mtxidx); }
	  else	if (mtxctr == 6) { WriteMtx(fileLine, Nrow, Ncol, fDiffTpiEnuXSecUncert,      mtxidx); }
   	  else	if (mtxctr == 7) { WriteMtx(fileLine, Nrow, Ncol, fDbldiffTmuCosmuXSec,	      mtxidx); }
	  else	if (mtxctr == 8) { WriteMtx(fileLine, Nrow, Ncol, fDbldiffTmuCosmuXSecUncert, mtxidx); }
   	  else	if (mtxctr == 9) { WriteMtx(fileLine, Nrow, Ncol, fDbldiffTpiCospiXSec,	      mtxidx); }
	  else	if (mtxctr == 10){ WriteMtx(fileLine, Nrow, Ncol, fDbldiffTpiCospiXSecUncert, mtxidx); }
   	}
      }
    }
  } else {
    LOG("MiniBooNE", pERROR) 
      << "Could not open file: " << data_file;
    return false;
  }

  // ROOT histogram requires bins in increasing order
  // Reversal of one set of bins requires the corresponding reversal of the data
 
  ReverseByRow(fDiffQ2EnuXSec,        CC1pip::kNDiffQ2EnuBins,     CC1pip::kNDiffQ2EnuQ2Bins );
  ReverseByRow(fDiffQ2EnuXSecUncert , CC1pip::kNDiffQ2EnuBins,     CC1pip::kNDiffQ2EnuQ2Bins );
  ReverseByRow(fDiffTmuEnuXSec,       CC1pip::kNDiffTmuEnuTmuBins, CC1pip::kNDiffTmuEnuBins  );
  ReverseByRow(fDiffTmuEnuXSecUncert, CC1pip::kNDiffTmuEnuTmuBins, CC1pip::kNDiffTmuEnuBins  );
  ReverseByRow(fDiffTpiEnuXSec ,      CC1pip::kNDiffTpiEnuTpiBins, CC1pip::kNDiffTpiEnuBins  );
  ReverseByRow(fDiffTpiEnuXSecUncert, CC1pip::kNDiffTpiEnuTpiBins, CC1pip::kNDiffTpiEnuBins  );

  ReverseByRow(fDbldiffTmuCosmuXSec,       CC1pip::kNDbldiffCosThetamuBins, CC1pip::kNDbldiffTmuBins );
  ReverseByRow(fDbldiffTmuCosmuXSecUncert, CC1pip::kNDbldiffCosThetamuBins, CC1pip::kNDbldiffTmuBins );
  ReverseByRow(fDbldiffTpiCospiXSec,       CC1pip::kNDbldiffCosThetapiBins, CC1pip::kNDbldiffTpiBins );
  ReverseByRow(fDbldiffTpiCospiXSecUncert, CC1pip::kNDbldiffCosThetapiBins, CC1pip::kNDbldiffTpiBins );

   return true;
}
//____________________________________________________________________________
bool MBCC1pipData::Initialise(double * data_array, int data_dims)
{
  for (int i = 0; i < data_dims ; i++){
    data_array[i] = 0;
  }
  return true;
}
//____________________________________________________________________________
void MBCC1pipData::ReplaceStringInPlace(std::string& subject, std::string& search, std::string& replace)
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
void MBCC1pipData::WriteMtx(std::string fileLine, int & Nrow, int & Ncol, double * xyBins, int & i)
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
      // y bins,

    } else if ( Nrow == 0 ) {
      // x bins

    } else {
      xyBins[i++] = kBins;

    }
    Ncol++;
  }
  
  Nrow++;
  Ncol = 0;
  //std::cout << std::endl; 
} 
//____________________________________________________________________________
void ReadLineToArray(double * XSecArray, double XSecVal, double * UncertArray,  double UncertVal, int & i)
{
  XSecArray[i]     = XSecVal ;     
  UncertArray[i++] = UncertVal ;  
}
//____________________________________________________________________________
// Data bins read by ROOT in ascending order, consequently need to reverse the row order
void MBCC1pipData::ReverseByRow(double * array, int width , int height)
{
  // Takes a one dimensional array with a width (number of columns) 
  // and height (number of rows, n) and swaps row i with row n-i 
  // element by element till the middle row of the array
  double temp;
  for (int j = 0 ; j < height/2. ; j++){ 
    for (int i = 0 ; i < width ; i++){
      temp = array[ width*j + i ];
      array[ width*j + i ] = array[ width*(height-j-1) + i ];
      array[ width*(height-j-1) + i ] = temp;
    }
  }

}
//____________________________________________________________________________
void MBCC1pipData::Print()
{

}
//____________________________________________________________________________
string MBCC1pipData::Name()
{

  return 0;
}
