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
#include <algorithm>

#include <TSystem.h>
#include <TChain.h>
#include <TIterator.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>

#include "DataComp/MiniBooNE/MBCCQEModel.h"
#include "DataComp/MiniBooNE/MBConstants.h"
#include "DataComp/MiniBooNE/MBDataRelease.h"
#include "Utils/GArray.h"
#include "EVGCore/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "Messenger/Messenger.h"
#include "Ntuple/NtpMCFormat.h"
#include "Ntuple/NtpMCTreeHeader.h"
#include "Ntuple/NtpMCEventRecord.h"
#include "PDG/PDGCodes.h"
#include "Utils/GSimFiles.h"
#include "Utils/XmlParserUtils.h"

using namespace genie;
using namespace genie::mc_vs_data;

//____________________________________________________________________________
MBCCQEModel::MBCCQEModel() :
GExModelABC()
{
  this->Init();
}
//____________________________________________________________________________
MBCCQEModel::~MBCCQEModel()
{
  delete fBinning;
  delete fGFileList;
  delete fGdDiffXSec;
}
//____________________________________________________________________________
string MBCCQEModel::Name(void)
{
  return MBDataRelease::AsString(fDataRelease);
}
//____________________________________________________________________________
const Registry & MBCCQEModel::Metadata(void) const
{
  return fMetadata;
}
//____________________________________________________________________________
bool MBCCQEModel::Read(string file, Option_t * opt)
{
  LOG("MiniBooNE", pDEBUG)
     << "Reading info to configure GENIE MiniBooNE CCQE cross-section model";

  // Read input XML file
  xmlDocPtr xml_doc = xmlParseFile(file.c_str());
  if(xml_doc==NULL) {
     LOG("MiniBooNE", pERROR)
       << "The XML file " << file << " can't be read.";
     return false;  
  }

  // Read data release info
  string release = utils::xml::GetString(xml_doc,"config/miniboone_data/release");
  fDataRelease = MBDataRelease::FromString(release);
  if(fDataRelease == kMBUndefined) {
     LOG("MiniBooNE", pERROR)
       << "Undefined data release";
     return false;
  }
  LOG("MiniBooNE", pNOTICE)
    << "Configuring object for data release: " << MBDataRelease::AsString(fDataRelease);

  bool paths_rel_genie = 
     utils::xml::GetBool(xml_doc,"config/miniboone_data/files/paths_relative_to_genie_topdir");
  string basedir = ""; 
  if(paths_rel_genie) {
     basedir = string( gSystem->Getenv("GENIE") ) + "/";
  }

  // Get data binning info
  fBinningFilename = basedir + utils::xml::GetString(xml_doc,"config/miniboone_data/files/binning");
  
  LOG("MiniBooNE", pNOTICE) << "Reading binning info from: " << fBinningFilename;
  bool read_ok = fBinning->Read(fBinningFilename);
  if(!read_ok) {
     return false;
  }

  // Flux medatadata
  fFluxFilename = utils::xml::GetROOTFileName(xml_doc,"config/miniboone_data/files/flux");   
  fFluxHistname = utils::xml::GetROOTObjName (xml_doc,"config/miniboone_data/files/flux"); 
  // Get MiniBooNE flux
  fFlux = utils::xml::GetTH1D(xml_doc,"config/miniboone_data/files/flux",basedir);
  if(!fFlux) {
     LOG("MiniBooNE", pERROR) << "No input flux histogram";
     return false;
  }

  // Get MiniBooNE fiducial mass and exposure relevant for the dataset GENIE is compared against
  fMass         = utils::xml::GetDouble(xml_doc,"config/miniboone_data/mass");
  fExposureData = utils::xml::GetDouble(xml_doc,"config/miniboone_data/exposure");

  // Get GENIE file list and chosen model name
  fGFileListFilename = utils::xml::GetString(xml_doc,"config/genie/files");
  fGModelName = utils::xml::GetString(xml_doc,"config/genie/model");

  LOG("MiniBooNE", pNOTICE) 
    << "Reading GENIE files for model = " << fGModelName 
    << "from " << fGFileListFilename;

  read_ok = fGFileList->LoadFromFile(fGFileListFilename);
  if(!read_ok) {
     return false;
  }

  // Get event chain
  fEventChain = fGFileList->EvtChain(0); /// HERE: USE CORRECT MODEL BASED ON NAME
  if(!fEventChain) return false;
  fEventChain->SetBranchAddress("gmcrec", &fMCRecord);

  // Work-out the input sample normalization
  this->Norm();

  // Set metadata 
  fMetadata.SetName(this->Name() + "_model_meta");
  fMetadata.Set("BinningFile",       fBinningFilename);
  fMetadata.Set("GenieFileList",     fGFileListFilename);
  fMetadata.Set("FluxFile",          fFluxFilename);
  fMetadata.Set("FluxHistogram",     fFluxHistname);
  fMetadata.Set("Mass_tonnes",       fMass);
  fMetadata.Set("ExposureData_pot",  fExposureData);
  fMetadata.Set("ExposureMC_pot",    fExposureMC);
  fMetadata.Set("MCNormScale",       fNormScale);
  fMetadata.Lock();


  return true;
}
//____________________________________________________________________________
bool MBCCQEModel::Generate(Option_t * opt)
{
  this->Reset();

  // Get bin edge arrays
  const double * costhetamu_bin_edges = fBinning->GetCosThetamuBinEdges();
  const double * Tmu_bin_edges        = fBinning->GetTmuBinEdges();
      
  // Number of entries
  Long_t nev = fEventChain->GetEntries();

  for(Long_t k = 0; k < nev; k++) {
    fEventChain->GetEntry(k);
    EventRecord & event = *(fMCRecord->event);

    bool   has_muon = false;  
    bool   has_pion = false;  
    double T_mu     = -999; // muon kinetic energy
    double costh_mu = -999; // muon cos(theta)

    GHepParticle * p = 0;
    TIter event_iter(&event);
    while((p=dynamic_cast<GHepParticle *>(event_iter.Next())))
    {  
       if (p->Status() != kIStStableFinalState) continue; 

       int pdgc = p->Pdg();
       if(pdgc == kPdgAntiMuon || pdgc == kPdgMuon) 
       {
         has_muon = true;
         T_mu = p->KinE();
         costh_mu = p->P4()->Pz() / p->P4()->P();
       }
       if (pdgc == kPdgPi0 || pdgc == kPdgPiP || pdgc == kPdgPiM) 
       {
         has_pion = true;
       }

    }// end loop over particles	
    
    // MB CCQE-like: Event has muon, no mesons, Tmu > cut
    //bool is_qelike = has_muon && (T_mu > MB::T_mu_cut) && !has_pion;
    bool is_qelike = has_muon && (T_mu > 0.2) && !has_pion;
    if(!is_qelike) continue;

    // put all CosThetamu below epsilon value in first bin 
    double epsilon = 1E-6;
    if(TMath::Abs(costh_mu-1) < epsilon) {
       costh_mu = costh_mu - epsilon;
    }

    int i = TMath::BinarySearch(CCQE::kNCosThetamuBins, costhetamu_bin_edges, costh_mu);
    int j = TMath::BinarySearch(CCQE::kNTmuBins,        Tmu_bin_edges,        T_mu    );            

    // Hook for reweighting used by tuning code, here weight is always 1
    double weight  = this->Weight(event); 
    // Event contribution to the double-differential xsec array
    double evt_contrib = fNormScale * weight;

    // Add weighted event to bin 
    fGdDiffXSec->Set(i,j, (evt_contrib + fGdDiffXSec->Get(i,j)) );

    fMCRecord->Clear();

  }//end loop over events

  return true;
}
//____________________________________________________________________________
bool MBCCQEModel::Write(
  TObjArray* array, const char * tag, Option_t * write_opt)
{
  LOG("MiniBooNE", pDEBUG) << "Writing results to input object array";

  if(!array) {
    return false;
  }

  string opt = (write_opt) ? write_opt : "";

  //
  // Write out the 2D data arrays
  //

  fGdDiffXSec -> SetName( Form("%s_%s_d2sig_arr", tag,this->Name().c_str()) );
  array -> Add (fGdDiffXSec);

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
    h[i] = new TH1D(name, title, CCQE::kNTmuBins, Tmu_bin_edges);
  }
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      double xsec_genie = fGdDiffXSec->Get(i,j);
      h[i]->SetBinContent(j+1,xsec_genie);
    }
  }
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    array->Add(h[i]); // transfers ownership?
  }

  // Create a ROOT TH2D histogram with the double differential cross-section
  const char * title2 = "";
  const char * name2  = Form("%s_%s_d2sig_h2d",tag,this->Name().c_str());
  TH2D * h2 = new TH2D(name2,title2,CCQE::kNCosThetamuBins,costhetamu_bin_edges,CCQE::kNTmuBins,Tmu_bin_edges);

  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      double d2sig = fGdDiffXSec->Get(i,j);
      h2->SetBinContent(i+1,j+1,d2sig);
    }
  }
  array->Add(h2);

  return true;
}
//____________________________________________________________________________
bool MBCCQEModel::Write(
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
void MBCCQEModel::Init(void) 
{
  fDataRelease        = kMBUndefined;
  fBinningFilename    = "";
  fGFileListFilename  = "";
  fGModelName         = "";
  fFluxFilename       = "";
  fFluxHistname       = "";

  fMass         = 0;
  fExposureData = 0;

  fFlux        = 0;
  fGFileList   = new GSimFiles;
  fEventChain  = 0;
  fMCRecord    = 0;
  fBinning     = new MBCCQEBinning;
  fGdDiffXSec  = new GArray2D(CCQE::kNCosThetamuBins, CCQE::kNTmuBins);

  fExposureMC   = 0;
  fNormScale    = 0;

  this->Reset();
}
//____________________________________________________________________________
void MBCCQEModel::Reset(void) 
{
  for(int i = 0; i < CCQE::kNCosThetamuBins; i++) {
    for(int j = 0; j < CCQE::kNTmuBins; j++) {
      fGdDiffXSec->Set(i,j, 0);
    }
  }
}
//____________________________________________________________________________
void MBCCQEModel::Print()
{
  // Get bin edge arrays
  const double * costhetamu_bin_edges = fBinning->GetCosThetamuBinEdges();
  const double * Tmu_bin_edges        = fBinning->GetTmuBinEdges();

  // Print formatted bin labels
  for (size_t j = 0; j < (size_t) CCQE::kNCosThetamuBins; j++){
    if (j == 0) {
      std::cout << std::setw(9) << "Bins" << "|" << std::setw(7);
      for (size_t n = 0; n < (size_t) CCQE::kNTmuBins; n++) {
	std::cout << std::setw(4) << Tmu_bin_edges[n] << "," 
		  << std::setw(3) << Tmu_bin_edges[n+1] << "|";
      }
      std::cout << '\n' ;
    }
    std::cout << std::setw(4) << costhetamu_bin_edges[j] << "," 
	      << std::setw(4) << costhetamu_bin_edges[j+1] << "|" ;
    
    //Loop over array components and print 
    for (size_t i = 0; i < (size_t) CCQE::kNTmuBins; i++){
      std::cout << std::setw(8) <<  fGdDiffXSec->Get(j,i) << " ";
    }
    std::cout << '\n';
  }
}
//____________________________________________________________________________
void MBCCQEModel::Norm(void)
{
//  N = Integral{
//	  (d3Flux / dE dS dI) * sig(E) * (Na/A) * rho*L * dE*dS*dI}
//    = Integral{
//        (d3Flux / dE dS dI) * sig(E) * (Na/A) * dE*dM*dI} 
// where
//   - d3Flux / dE dS dI: numu flux per energy bin, per unit area, per POT
//   - sigma(E): total numu cross section on water
//   - Na: Avogadro's number
//   - A: mass number fir CH2
//   - rho: density
//   - L: path length
//   - M: mass (fiducial)
//   - I: beam intensity (POT)

  TFile * f_xsec = fGFileList->XSecFile(0); /// /// HERE: USE CORRECT MODEL BASED ON NAME

  TDirectoryFile * dirf_h1  = 0;
  TDirectoryFile * dirf_c12 = 0;

  if(fDataRelease == kMBCCQENu2010) 
  {
    dirf_h1  = (TDirectoryFile *) f_xsec -> Get("nu_mu_H1");
    dirf_c12 = (TDirectoryFile *) f_xsec -> Get("nu_mu_C12"); 
  } 
  else 
  if(fDataRelease == kMBCCQENuBar2013) 
  {
    dirf_h1  = (TDirectoryFile *) f_xsec -> Get("nu_mu_bar_H1");
    dirf_c12 = (TDirectoryFile *) f_xsec -> Get("nu_mu_bar_C12"); 
  } 
  assert(dirf_h1);
  assert(dirf_c12);

  TGraph * h_ccxsec_h1  = (TGraph*) dirf_h1  -> Get("tot_cc");
  TGraph * h_ccxsec_c12 = (TGraph*) dirf_c12 -> Get("tot_cc");
  TGraph * h_ncxsec_h1  = (TGraph*) dirf_h1  -> Get("tot_nc");
  TGraph * h_ncxsec_c12 = (TGraph*) dirf_c12 -> Get("tot_nc"); ///gspl2root should be writting out tot cc+nc too
  assert(h_ccxsec_h1);
  assert(h_ccxsec_c12);
  assert(h_ncxsec_h1);
  assert(h_ncxsec_c12);

  double n = 0;
  double flux_sum = 0;

  for(int ibin = 0; ibin < fFlux->GetNbinsX(); ibin++) {
    // get flux bin content
    double flux = fFlux->GetBinContent(ibin);  // #nu /cm^2 /GeV /POT 
    double dE   = fFlux->GetBinWidth(ibin);    // typically 50-MeV bins are used
    double Elo  = fFlux->GetBinLowEdge(ibin); 
    double Ehi  = Elo + dE;

    // eval xsec at bin centre
    double E = fFlux->GetBinCenter(ibin); 
    double xsec_h1  = h_ccxsec_h1 ->Eval(E) + h_ncxsec_h1 ->Eval(E);
    double xsec_c12 = h_ccxsec_c12->Eval(E) + h_ncxsec_c12->Eval(E);
    double xsec     = (E > 0) ? 2 * xsec_h1 + xsec_c12 : 0.;

    LOG("MiniBooNE", pNOTICE) 
      << "flux (E = [" << Elo << ", " << Ehi << "] GeV) = " << flux
      << " #nus /cm^2 /GeV /POT, xsec_total(CH2, E = " << E << " GeV) = " << xsec << " x 1E-38 cm^2";

    double dn = flux * xsec;
    n += dn;
    flux_sum += (flux*dE);
  }

  flux_sum *= fExposureData;
  LOG("MiniBooNE", pDEBUG)  
     << "Flux integral (compare against value in paper) = " << flux_sum << "#nu / cm^2";

  // Multiply n with appropriate factor to convert to events/POT
  // (re-factor to avoid rounding errors from the multiplication of very big numbers with very small numbers)
  //
  double Na     = 6.023 * 1E+23;  // 
  double M      = fMass * 1E+06;  // in gr
  double A      =    14;              // in gr
  double to_cm2 =     1 * 1E-38;  // factor needed to convert gspl2root xsecs to cm^2

  n *= (fExposureData * Na * to_cm2 * M / A);

  LOG("MiniBooNE", pNOTICE) 
      << "Model predicts " << n << " CC+NC events for an exposure of " 
      << fExposureData << " POT";

  // Number of entries in input compherensive event sample
  Long_t nev = fEventChain->GetEntries();

  // MC sample exposure in terms of POT
  fExposureMC = (nev/n)*fExposureData;

  LOG("MiniBooNE", pNOTICE) 
      << "Input sample has " << nev << " CC+NC events: "
      << "It corresponds to an exposure of " << fExposureMC << " POT";

  // Factor to convert number of events to nuclear cross-section
  fNormScale = n/nev; // basically = fExposureData/fExposureMC;

  int Nhit = 1;
  if      (fDataRelease == kMBCCQENu2010   ) Nhit = 6; // number of neutrons
  else if (fDataRelease == kMBCCQENuBar2013) Nhit = 8; // number of protons

  // Factor to convert number of events to cross-section per neutron or
  // proton, as appropriate
  fNormScale /= Nhit;

  LOG("MiniBooNE", pNOTICE) 
      << "MC normalization scale = " << fNormScale;
}
//____________________________________________________________________________

