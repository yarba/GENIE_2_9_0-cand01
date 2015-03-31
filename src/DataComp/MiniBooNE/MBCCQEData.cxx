//____________________________________________________________________________
/*
 Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Chris Barry <cbarry \at hep.ph.liv.ac.uk>
         University of Liverpool

         Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Laboratory
*/
//____________________________________________________________________________

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <TSystem.h>
#include <TObjArray.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>

#include "DataComp/Base/GExModelABC.h"
#include "DataComp/MiniBooNE/MBCCQEData.h"
#include "DataComp/MiniBooNE/MBCCQEBinning.h"
#include "Messenger/Messenger.h"
#include "Utils/StringUtils.h"
#include "Utils/XmlParserUtils.h"
#include "Utils/GArray.h"

using namespace genie;
using namespace genie::mc_vs_data;

//____________________________________________________________________________
MBCCQEData::MBCCQEData() :
GExDataABC()
{
  this->Init();
}
//____________________________________________________________________________
MBCCQEData::~MBCCQEData()
{
  delete fBinning;
  delete fdDiffXSecCentrVal;  
  delete fdDiffXSecShapeErr;
}
//____________________________________________________________________________
string MBCCQEData::Name(void)
{
  return MBDataRelease::AsString(fDataRelease);
}
//____________________________________________________________________________
const Registry & MBCCQEData::Metadata(void) const
{
  return fMetadata;
}
//____________________________________________________________________________
bool MBCCQEData::Read(string file, Option_t * opt)
{
  LOG("MiniBooNE", pDEBUG) 
     << "Reading MiniBooNE double-differential CCQE cross-sections";

  xmlDocPtr xml_doc = xmlParseFile(file.c_str());
  if(xml_doc==NULL) {
     LOG("MiniBooNE", pERROR)
       << "The XML file " << file << " can't be read.";
     return false;
  }

  string release = utils::xml::GetString(xml_doc,"config/miniboone_data/release");
  fDataRelease = MBDataRelease::FromString(release);
  if(fDataRelease == kMBUndefined) {
     LOG("MiniBooNE", pERROR)
       << "Undefined data release";
     return false;
  }
  LOG("MiniBooNE", pNOTICE) 
    << "Configuring object for data release: " << MBDataRelease::AsString(fDataRelease);

  fBinningFilename        = utils::xml::GetString(xml_doc,"config/miniboone_data/files/binning");
  fXSecDataFilename       = utils::xml::GetString(xml_doc,"config/miniboone_data/files/xsec_central_values");
  fXSecShapeErrorFilename = utils::xml::GetString(xml_doc,"config/miniboone_data/files/xsec_shape_errors");

  bool paths_rel_genie = utils::xml::GetBool(xml_doc,"config/miniboone_data/files/paths_relative_to_genie_topdir");
  if(paths_rel_genie) {
     string basedir = string( gSystem->Getenv("GENIE") );
     fBinningFilename        = basedir + "/" + fBinningFilename;
     fXSecDataFilename       = basedir + "/" + fXSecDataFilename;
     fXSecShapeErrorFilename = basedir + "/" + fXSecShapeErrorFilename;
  }
  
  bool read_ok = false;

  // Read binning
  LOG("MiniBooNE", pNOTICE) 
     << "Reading binning info from: " << fBinningFilename;
  read_ok = fBinning->Read(fBinningFilename);
  if(!read_ok) {
     return false;
  }
 
  // Read central values   
  LOG("MiniBooNE", pNOTICE) 
     << "Reading central values from: " << fXSecDataFilename;
  read_ok = this->ReadMBFile(fXSecDataFilename, fdDiffXSecCentrVal);
  if(!read_ok) {
     return false;
  }

  // Read shape error
  LOG("MiniBooNE", pNOTICE) 
     << "Reading shape errors from: "<< fXSecShapeErrorFilename;
  this->ReadMBFile(fXSecShapeErrorFilename, fdDiffXSecShapeErr);
  if(!read_ok) {
     return false;
  }

  // Set metadata
  fMetadata.SetName(this->Name() + "_data_meta");
  fMetadata.Set("BinningFile",           fBinningFilename);
  fMetadata.Set("XSecCentralValuesFile", fXSecDataFilename);
  fMetadata.Set("XSecShapeErrorFile",    fXSecShapeErrorFilename);
  fMetadata.Lock();

  // Check
  this->Print();

  return true;
}
//____________________________________________________________________________
bool MBCCQEData::Generate(GExModelABC * /*model*/, bool /*fluctuate*/)
{
  return false;
}
//____________________________________________________________________________
bool MBCCQEData::Write(
  TObjArray * array, const char * tag, Option_t * write_opt)
{
  LOG("MiniBooNE", pDEBUG) << "Writing results to input object array";

  if(!array) {
    return false;
  }

  string opt = (write_opt) ? write_opt : "";

  //
  // Write out the 2D data arrays
  //

  fdDiffXSecCentrVal -> SetName( Form("%s_%s_d2sig_arr",          tag,this->Name().c_str()) ); 
  fdDiffXSecShapeErr -> SetName( Form("%s_%s_d2sig_shape_err_arr",tag,this->Name().c_str()) ); 

  array -> Add (fdDiffXSecCentrVal);
  array -> Add (fdDiffXSecShapeErr);

  //
  // For visualization purposes, if asked, convert data array to TH2D format
  // and, additionally, generate TH1D slices
  //

  bool save_root = 
     ( opt.find("all")  != string::npos ) ||
     ( opt.find("ROOT") != string::npos );

  if(!save_root) return true;

  // Get bin edge arrays
  const double * costhetamu_bin_edges = fBinning->GetCosThetamuBinEdges(); 
  const double * Tmu_bin_edges        = fBinning->GetTmuBinEdges();

  // Create a series of ROOT TH1D histograms, one for every cos(theta_mu) 
  // slice, to store cross-section as function of Tmu 
  TH1D * h[CCQE::kNCosThetamuBins];
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    const char * title = "";
    const char * name  = Form("%s_%s_d2sig_costh%d_h1d",tag,this->Name().c_str(),i);
    h[i] = new TH1D(name,title,CCQE::kNTmuBins,Tmu_bin_edges);
    h[i]->SetTitle(MBDataRelease::AsRooTeX(fDataRelease));
  }
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      double xsec = fdDiffXSecCentrVal->Get(i,j);
      h[i]->SetBinContent(j+1,xsec);
    }
  }
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    array->Add(h[i]); // transfers ownership?
  }

  // Create a ROOT TH2D histogram with the double differential cross-section
  const char * title2 = "";
  const char * name2  = Form("%s_%s_d2sig_h2d",tag,this->Name().c_str());
  TH2D * h2 = new TH2D(name2,title2,CCQE::kNTmuBins,costhetamu_bin_edges, CCQE::kNTmuBins,Tmu_bin_edges);
    
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      double d2sig = fdDiffXSecCentrVal->Get(i,j);
      h2->SetBinContent(i+1,j+1,d2sig);
    }
  }
  array->Add(h2);

  return true;
}
//____________________________________________________________________________
bool MBCCQEData::Write(
  const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{  
  LOG("MiniBooNE", pNOTICE) << "Writing results to file: [" << filename << "]";

  TObjArray * array = new TObjArray();
  this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();

  return true;
}
//____________________________________________________________________________
int MBCCQEData::NumOfBins (void)
{
// Count number of bins

  int nbins = 0;
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      if(fdDiffXSecCentrVal->Get(i,j) > 0) { nbins++; }
    }
  }
  return nbins;
}
//____________________________________________________________________________
void MBCCQEData::Init()
{
  fDataRelease = kMBUndefined;

  fBinningFilename        = "";
  fXSecDataFilename       = "";
  fXSecShapeErrorFilename = "";
  
  fBinning = new MBCCQEBinning;

  fdDiffXSecCentrVal = new GArray2D(CCQE::kNCosThetamuBins, CCQE::kNTmuBins);
  fdDiffXSecShapeErr = new GArray2D(CCQE::kNCosThetamuBins, CCQE::kNTmuBins);

  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      fdDiffXSecCentrVal->Set (i,j, 0);
      fdDiffXSecShapeErr->Set (i,j, 0); 
    }
  }
}
//____________________________________________________________________________
void MBCCQEData::Print(void)
{
  LOG("MiniBooNE", pNOTICE) 
     << "Double-differential cross-section central values: ";
  this->Print(fdDiffXSecCentrVal);

  LOG("MiniBooNE", pNOTICE) 
     << "Double-differential cross-section shape error (fractional): ";
  this->Print(fdDiffXSecShapeErr);
}
//____________________________________________________________________________
void MBCCQEData::Print(GArray2D * data_array)
{
  // Get bin edge arrays
  const double * costhetamu_bin_edges = fBinning->GetCosThetamuBinEdges(); 
  const double * Tmu_bin_edges        = fBinning->GetTmuBinEdges();

  // print array contents to screen
  // print formatted bin labels
  for (size_t i = 0; i < (size_t) CCQE::kNCosThetamuBins; i++){
    if (i == 0) {
      std::cout << std::setw(9) << "Bins" << "|" << std::setw(7);
      for (size_t n = 0; n < (size_t) CCQE::kNTmuBins; n++) {
	std::cout << std::setw(4) << Tmu_bin_edges[n] << "," 
		  << std::setw(3) << Tmu_bin_edges[n+1] << "|";
      }
      std::cout << '\n' ;
    }
    std::cout << std::setw(4) << costhetamu_bin_edges[i] << "," 
	      << std::setw(4) << costhetamu_bin_edges[i+1] << "|" ;
    
    // Loop over array components and print 
    for (size_t j = 0; j < (size_t) CCQE::kNTmuBins; j++){
      std::cout << std::setw(8) <<  data_array->Get(i,j) << " ";
    }
    std::cout << '\n';
  }
}
//____________________________________________________________________________
bool MBCCQEData::ReadMBFile(
    string file_name, GArray2D * data_array)
{
  int i=CCQE::kNCosThetamuBins-1;
  int j=0;                       
  
  ifstream ifile(file_name.c_str());
  if ( ifile.is_open() ){
    if ( ifile.good() ){
      string fileLine;
      double xsec = 0;
      string kNoData = "---", kReplace = "0";
      while ( getline(ifile,fileLine) ){

	// Replace "---" in file with 0 to parse with stringstream
	utils::str::ReplaceStringInPlace(fileLine, kNoData, kReplace);
	std::stringstream fStrStream(fileLine);
      
	// Read into last value of array, then decrease index 
        while (fStrStream >> xsec) {
          data_array->Set(i,j++,xsec);
	  if(j == CCQE::kNTmuBins) {
	    i--;
            j = 0;
          }
	}
      }
    }
  } else {
    LOG("MiniBooNE", pERROR) 
        << "Could not open file: " << file_name;
    return false;
  }
  ifile.close();
  return true;
}
//____________________________________________________________________________

