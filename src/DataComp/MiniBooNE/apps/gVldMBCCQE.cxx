//___________________________________________________________________________________________
/*!

\program gvld_miniboone_ccqe

\brief   A validation app comparing GENIE with the MiniBooNE CCQE cross-section data.
               
         gvld_miniboone_ccqe -d data_config.xml [-g genie_model_config.xml] [-o out_file]

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
              Default: mbccqe
             
\author  Chris Barry <cbarry \at hep.ph.liv.ac.uk>
         University of Liverpool

         Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         University of Liverpool & STFC Rutherford Appleton Laboratory

\created Jan 11, 2015 

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

#include "DataComp/MiniBooNE/MBCCQEBinning.h"
#include "DataComp/MiniBooNE/MBCCQEData.h"
#include "DataComp/MiniBooNE/MBCCQEModel.h"
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

  GExDataABC * data = new MBCCQEData;
  bool ok = data->Read(gOptDataConfig);
  if(!ok) {
    LOG("gvld_miniboone_ccqe", pFATAL) 
      << "MBCCQEData not configured properly. Check " << gOptDataConfig;
    exit(1);
  }
  LOG("gvld_miniboone_ccqe", pNOTICE) 
      << "MBCCQEData metadata:\n " << data->Metadata();

  //
  // Instantiate and configure model
  //

  GExModelABC * model = 0;
  if(gOptShowModel) {
     model = new MBCCQEModel;    
     ok = model->Read(gOptModelConfig);
     if(!ok) {
        LOG("gvld_miniboone_ccqe", pFATAL) 
           << "MBCCQEModel not configured properly. Check " << gOptModelConfig;
        exit(1);
     }
     LOG("gvld_miniboone_ccqe", pNOTICE) 
      << "MBCCQEModel metadata:\n " << model->Metadata();

     // Generate nominal model prediction
     model->Generate();
  }

  //
  // Extract the data and nominal predictions 
  //

  TObjArray * array = new TObjArray();
  data->Write(array, "data", "all");
  if(gOptShowModel) {
    model->Write(array, "genie", "all");
  }

  //
  // Write everything out in a ROOT file
  //

  // Get local time to tag outputs
  //string lt_for_filename   = utils::system::LocalTimeAsString("%02d.%02d.%02d_%02d.%02d.%02d");
  string lt_for_cover_page = utils::system::LocalTimeAsString("%02d/%02d/%02d %02d:%02d:%02d");

  string output_filename_root = gOptOutFilename + ".root";
  TFile f(output_filename_root.c_str(), "recreate");
  array->Write();
  f.Close();

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
  hdr.AddText("GENIE comparison with MiniBooNE QE data");
  hdr.AddText(" ");
  hdr.AddText(" ");
  hdr.AddText(" ");
  hdr.AddText(" ");
  hdr.AddText(lt_for_cover_page.c_str());
  hdr.AddText(" ");
  hdr.Draw();
  c->Update();

  const int kNCosThetamuBins = 20;

  const int kNCx = 2; // columns and rows in canvas
  const int kNCy = 2;
  const int kNPlotsPerPage = kNCx * kNCy;

  for(int i = 0; i < kNCosThetamuBins; i++) {
     int iplot = 1 + i % kNPlotsPerPage;
     if(iplot == 1) {
       ps->NewPage();
       c -> Clear();
       c -> Divide(kNCx,kNCy);
     }
     c -> GetPad(iplot) -> Range(0,0,100,100);
     c -> GetPad(iplot) -> SetFillColor(0);
     c -> GetPad(iplot) -> SetBorderMode(0);
     c -> GetPad(iplot) -> cd();

     TH1D * hdata  = (TH1D*) array->FindObject( Form("data_%s_d2sig_costh%d_h1d", data ->Name().c_str(),i) );
     TH1D * hmodel = 0;
     if(gOptShowModel) {
        hmodel = (TH1D*) array->FindObject( Form("genie_%s_d2sig_costh%d_h1d",model->Name().c_str(),i) );
     }
     hdata->GetXaxis()->SetTitle("T_{#mu} (GeV)");
     hdata->GetYaxis()->SetTitle("d^{2}#sigma/dcos#theta_{#mu}dT_{#mu} (cm^{2}/GeV)");

     hdata->Draw("perr");
     if(gOptShowModel) { hmodel->Draw("same"); }

     TLegend * leg = new TLegend(0.4, 0.6, 0.9, 0.9);
     leg->SetHeader("cos#theta = [?,?]");
     leg->AddEntry(hdata,  hdata->GetTitle(),  "p");
     if(gOptShowModel) { leg->AddEntry(hmodel, "GENIE", "l"); }
     leg->SetTextSize(0.03);
     leg->SetFillColor(0);
     leg->Draw();

     c->GetPad(iplot)->Update();
     c->Update();
  }

  ps->Close();
  delete c;
  delete ps;

  return 0;
}
//___________________________________________________________________________________________
void GetCommandLineArgs (int argc, char ** argv)
{
  LOG("gvld_miniboone_ccqe", pNOTICE) << "Parsing command line arguments";

  CmdLnArgParser parser(argc,argv);

  // get data
  if(parser.OptionExists('d')){
     string filename = parser.ArgAsString('d');
     gOptDataConfig = filename;
     LOG("gvld_miniboone_ccqe", pNOTICE)
       << "Reading MiniBooNE data release information from: " << gOptDataConfig;
  } else {
     LOG("gvld_miniboone_ccqe", pFATAL)
          << "Please specify MiniBooNE data release information using the -d option.";
     gAbortingInErr = true;
     exit(1);
  }

  // get GENIE inputs
  gOptShowModel = false;
  if( parser.OptionExists('g') ) {
     gOptModelConfig = parser.ArgAsString('g');
     gOptShowModel = true;
     LOG("gvld_miniboone_ccqe", pNOTICE)
       << "Reading GENIE config from: " << gOptModelConfig;
  } 

  gOptOutFilename = "mbccqe";
  if( parser.OptionExists('o') ) {
     gOptOutFilename = parser.ArgAsString('o');
     LOG("gvld_miniboone_ccqe", pNOTICE)
       << "Storing outputs in: " << gOptOutFilename << ".root/.ps";
  } 

}
//___________________________________________________________________________________________
