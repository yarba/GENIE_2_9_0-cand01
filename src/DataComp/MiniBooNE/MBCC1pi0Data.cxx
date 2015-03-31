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

#include "DataComp/MiniBooNE/MBCC1pi0Data.h"

using namespace genie;
using namespace genie::mc_vs_data;
using namespace genie::mc_vs_data::constants::MB;

//____________________________________________________________________________
MBCC1pi0Data::MBCC1pi0Data(MBCC1pi0Binning & binning)
{
  //  std::string fCC1pi0Path = "/data/validation/vA/xsec/differential/numu_ccpi0"; 
  //  std::string GeniePath = std::getenv ("GENIE");
  //  std::string CC1pi0Path = GeniePath + fCC1pi0Path;
  std::string CC1pi0Path = "ccpi0/";

  fEnuData_file          = CC1pi0Path + "totalxsec.txt";
  fQ2Data_file           = CC1pi0Path + "dxsecdq2.txt";
  fEmuData_file          = CC1pi0Path + "dxsecdemu.txt";
  fCosThetamuData_file   = CC1pi0Path + "dxsecdcosmu.txt";
  fPpi0Data_file         = CC1pi0Path + "dxsecdppi.txt";
  fCosThetapi0Data_file  = CC1pi0Path + "dxsecdcospi.txt";

  // Set bins
  fEnuBinEdges         = binning.GetEnuBinEdges()         ;
  fQ2BinEdges          = binning.GetQ2BinEdges()          ;
  fEmuBinEdges         = binning.GetEmuBinEdges()         ;
  fCosThetamuBinEdges  = binning.GetCosThetamuBinEdges()  ;
  fPpi0BinEdges        = binning.GetPpi0BinEdges()        ;
  fCosThetapi0BinEdges = binning.GetCosThetapi0BinEdges() ;

  // initialise arrays holding binning information
  
  Initialise( fEnuXsec        , CC1pi0::kNEnuBins         + 1 );
  Initialise( fQ2Xsec         , CC1pi0::kNQ2Bins          + 1 );
  Initialise( fEmuXsec        , CC1pi0::kNEmuBins         + 1 );
  Initialise( fCosThetamuXsec , CC1pi0::kNCosThetamuBins  + 1 );
  Initialise( fPpi0Xsec       , CC1pi0::kNPpi0Bins        + 1 );
  Initialise( fCosThetapi0Xsec, CC1pi0::kNCosThetapi0Bins + 1 );

  Initialise( fEnuCov        , (CC1pi0::kNEnuBins         * CC1pi0::kNEnuBins        ) );   
  Initialise( fQ2Cov         , (CC1pi0::kNQ2Bins          * CC1pi0::kNQ2Bins         ) );
  Initialise( fEmuCov        , (CC1pi0::kNEmuBins         * CC1pi0::kNEmuBins        ) );
  Initialise( fCosThetamuCov , (CC1pi0::kNCosThetamuBins  * CC1pi0::kNCosThetamuBins ) );
  Initialise( fPpi0Cov       , (CC1pi0::kNPpi0Bins        * CC1pi0::kNPpi0Bins       ) );
  Initialise( fCosThetapi0Cov, (CC1pi0::kNCosThetapi0Bins * CC1pi0::kNCosThetapi0Bins) );

}
//____________________________________________________________________________
MBCC1pi0Data::~MBCC1pi0Data()
{

}
//____________________________________________________________________________
void MBCC1pi0Data::Write(const char * output_file)
{
  // Create new file for each run of Write()
  TFile f(output_file,"recreate");
  f.Close();
  
  WriteSingle( fEnuXsec        , fEnuBinEdges        , CC1pi0::kNEnuBins        , "data_EnuXsec"    , output_file );        
  WriteSingle( fQ2Xsec         , fQ2BinEdges         , CC1pi0::kNQ2Bins         , "data_Q2Xsec"	    , output_file );
  WriteSingle( fEmuXsec        , fEmuBinEdges        , CC1pi0::kNEmuBins        , "data_EmuXsec"    , output_file );
  WriteSingle( fCosThetamuXsec , fCosThetamuBinEdges , CC1pi0::kNCosThetamuBins , "data_CosThetamu" , output_file );
  WriteSingle( fPpi0Xsec       , fPpi0BinEdges       , CC1pi0::kNPpi0Bins       , "data_Ppi0Xsec"   , output_file );
  WriteSingle( fCosThetapi0Xsec, fCosThetapi0BinEdges, CC1pi0::kNCosThetapi0Bins, "data_CosThetapi0", output_file );
  
  
   
}
//____________________________________________________________________________
// Takes data and bin edges and a label to write to root file
void MBCC1pi0Data::WriteSingle(double * array, const double * edges_array, const int array_bound, std::string name_label, const char * output_file)
{
  std::string append = "_%d";
  TH1D * histo;
  
  const char * title = "";
  std::string concat_label = name_label + append;
  const char * name_data = concat_label.c_str();
  histo = new TH1D(name_data, title, array_bound, edges_array);

  // Fill root histogram with the data  
  for(int i = 0; i < array_bound; i++) {
    double xsec = array[i];
    histo->SetBinContent(i+1,xsec);
  }

  // Use ROOT TFile Class to write data
  TFile f(output_file,"update");
  for(int i = 0; i < array_bound; i++) {
    histo->Write();
  }
  f.Close();
  
}
//____________________________________________________________________________
void MBCC1pi0Data::Print()
{
  // Write Data to screen
}
//____________________________________________________________________________
// Pass in string, read next line of text data file
bool MBCC1pi0Data::ReadArray(std::string BinEdgesFile, double * data_array, std::string read_string)
{
  // Files have same structure
  // Read line which has read_string
  
  std::ifstream dDiffXSecFile(BinEdgesFile.c_str());
  
  if (dDiffXSecFile.is_open()){
    
    if (dDiffXSecFile.good()){
      std::string fileLine;
      double kdDiffXSec(0);
      int counter = 0 , blanklines = 0, i = 0 ;
      while ( getline( dDiffXSecFile, fileLine ) ){
	// if file has char "read_string" on line then increment counter
	if ( fileLine.find( read_string, 0 ) != std::string::npos ) {
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
	    data_array[i++] = kdDiffXSec ;
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
bool MBCC1pi0Data::Read()
{
  std::string XsecCVal = "Central Value";
  ReadArray( fEnuData_file        , fEnuXsec         , XsecCVal);
  ReadArray( fQ2Data_file         , fQ2Xsec          , XsecCVal);
  ReadArray( fEmuData_file        , fEmuXsec         , XsecCVal);
  ReadArray( fCosThetamuData_file , fCosThetamuXsec  , XsecCVal);
  ReadArray( fPpi0Data_file       , fPpi0Xsec        , XsecCVal);
  ReadArray( fCosThetapi0Data_file, fCosThetapi0Xsec , XsecCVal);

  std::string XSecCov = "Systematic Uncertainty Correlation";
  ReadArray( fEnuData_file        , fEnuCov        , XSecCov );
  ReadArray( fQ2Data_file         , fQ2Cov         , XSecCov );
  ReadArray( fEmuData_file        , fEmuCov        , XSecCov );
  ReadArray( fCosThetamuData_file , fCosThetamuCov , XSecCov );
  ReadArray( fPpi0Data_file       , fPpi0Cov       , XSecCov );
  ReadArray( fCosThetapi0Data_file, fCosThetapi0Cov, XSecCov );
  return true;
}
//____________________________________________________________________________
bool MBCC1pi0Data::Initialise(double * data_array, int data_dims)
{
  for (int i = 0; i < data_dims ; i++){
    data_array[i] = 0;
  }
  return true;
}
//____________________________________________________________________________
