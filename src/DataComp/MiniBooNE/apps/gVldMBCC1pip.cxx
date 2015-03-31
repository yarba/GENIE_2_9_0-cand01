//___________________________________________________________________________________________
/*!

\program gvld_miniboone_cc1pip

\brief   A validation app comparing GENIE with the MiniBooNE CC1pi+ cross-section data.
         
         gvld_miniboone_cc1pip -d data_config.xml [-g genie_model_config.xml] [-o out_file]
         
         Options:
           
           [] Denotes an optional argument.
           
           -d An XML file with information about the MiniBooNE data release
              (provides locations and names of files with information on binning, the
               cross-section central values and errors etc)
              For examples, see: $GENIE/data/validation/vA/xsec/differential/
           
           -g An XML file specifying all the GENIE input files as well as necessary
              info about the data that predictions will be compared against (binning etc).
              For examples, see: $GENIE/data/validation/vA/xsec/differential/  
              If a -g argument is not specified, only the data will be plotted.
              Currently a single model prediction is plotted.
              TODO: Overlay all requested model predictions.
           
           -o Name of output ROOT and postscript files.
              Default: mbcc1pip

\author  Chris Barry <cbarry \at hep.ph.liv.ac.uk>
         University of Liverpool
         
         Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Laboratory

\cpright Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
         For the full text of the license visit http://copyright.genie-mc.org
         or see $GENIE/LICENSE
*/
//___________________________________________________________________________________________

#include <string>
#include <cstdlib>
         
#include <TFile.h>
#include <TROOT.h>
#include <TPostScript.h>
#include <TPavesText.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TObjArray.h>
#include <TH1D.h>
#include <TH2D.h>
              
#include "DataComp/MiniBooNE/MBCC1pipBinning.h"
#include "DataComp/MiniBooNE/MBCC1pipData.h"
#include "DataComp/MiniBooNE/MBCC1pipModel.h"
#include "Messenger/Messenger.h"
#include "Utils/CmdLnArgParser.h"
#include "Utils/GSimFiles.h"
#include "Utils/SystemUtils.h"
#include "Utils/RunOpt.h"
#include "Utils/Style.h"
         
using std::string;
         
using namespace genie;
using namespace genie::mc_vs_data;

string gOptDataConfig;
string gOptModelConfig;
string gOptOutFilename;
bool   gOptShowModel;
    
void GetCommandLineArgs (int argc, char ** argv);

int main(int argc, char ** argv)
{   
  GetCommandLineArgs(argc,argv);

  // Set GENIE style
  utils::style::SetDefaultStyle();
     
  //
  // Instantiate and read data
  //
/*           
  GExDataABC * data = new MBCC1pipData;
  bool ok = data->Read(gOptDataConfig);
  if(!ok) {
    LOG("gvld_miniboone_cc1pip", pFATAL)
      << "MBCC1pipData not configured properly. Check " << gOptDataConfig;
    exit(1);
  } 
*/
  //
  // Instantiate and configure model
  //
/*  
  GExModelABC * model = 0;
  if(gOptShowModel) {
     model = new MBCC1pipModel;
     ok = model->Read(gOptModelConfig);
     if(!ok) {
        LOG("gvld_miniboone_cc1pip", pFATAL)
           << "MBCC1pipModel not configured properly. Check " << gOptModelConfig;
        exit(1);
     }
     // Generate nominal model prediction
     model->Generate();
  }
*/
  //
  // Extract the data and nominal predictions
  //
/*
  TObjArray * array = new TObjArray();
  data->Write(array, "data", "all");
  if(gOptShowModel) {
    model->Write(array, "genie", "all");
  }
*/
  //
  // Write everything out in a ROOT file
  //

  // Get local time to tag outputs
  //string lt_for_filename   = utils::system::LocalTimeAsString("%02d.%02d.%02d_%02d.%02d.%02d");
  string lt_for_cover_page = utils::system::LocalTimeAsString("%02d/%02d/%02d %02d:%02d:%02d");
/*
  string output_filename_root = gOptOutFilename + ".root";
  TFile f(output_filename_root.c_str(), "recreate");
  array->Write();

  TFile f(output_filename_root.c_str(), "recreate");
  array->Write();
  f.Close();
*/  
  //
  // For convenience, add a set of publication-quality plots in a PS file
  //
     
  string output_filename_ps = gOptOutFilename + ".ps";
  TPostScript * ps = new TPostScript(output_filename_ps.c_str(), 111);
        
  // Create plot canvas
  TCanvas * c = new TCanvas("c","",20,20,500,650);
  c->SetBorderMode(0); 
  c->SetFillColor(0);
  c->SetGridx();
  c->SetGridy();
  
  // Add cover page
  ps->NewPage();
  c->Range(0,0,100,100);
  TPavesText hdr(10,40,90,70,3,"tr");
  hdr.AddText(" ");  
  hdr.AddText("GENIE comparison with MiniBooNE CC1pi+ data");
  hdr.AddText(" ");
  hdr.AddText(" ");
  hdr.AddText(" ");
  hdr.AddText(" ");
  hdr.AddText(lt_for_cover_page.c_str());
  hdr.AddText(" ");
  hdr.Draw();
  c->Update();

  // 
  //  ...
  //  ... actual plotting
  //  ...
  //  ...
  // 

  ps->Close();
  delete c;
  delete ps;
       
  return 0;
}
//___________________________________________________________________________________________
void GetCommandLineArgs (int argc, char ** argv)
{
  LOG("gvld_miniboone_cc1pip", pNOTICE) << "Parsing command line arguments";
     
  CmdLnArgParser parser(argc,argv);

  // get data
  if(parser.OptionExists('d')){
     string filename = parser.ArgAsString('d');
     gOptDataConfig = filename;
     LOG("gvld_miniboone_cc1pip", pNOTICE)
       << "Reading MiniBooNE data release information from: " << gOptDataConfig;
  } else {
     LOG("gvld_miniboone_cc1pip", pFATAL)
          << "Please specify MiniBooNE data release information using the -d option.";
     gAbortingInErr = true;
     exit(1);
  }

  // get GENIE inputs
  gOptShowModel = false;
  if( parser.OptionExists('g') ) {
     gOptModelConfig = parser.ArgAsString('g');
     gOptShowModel = true;
     LOG("gvld_miniboone_cc1pip", pNOTICE)
       << "Reading GENIE config from: " << gOptModelConfig;
  }  
       
  gOptOutFilename = "mbcc1pip";
  if( parser.OptionExists('o') ) {
     gOptOutFilename = parser.ArgAsString('o');
     LOG("gvld_miniboone_cc1pip", pNOTICE)
       << "Storing outputs in: " << gOptOutFilename << ".root/.ps";
  }
  
} 
//___________________________________________________________________________________________
  
     

