void mbqe_plots(char * filename) {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetLineScalePS(1.5);
  TGaxis::SetMaxDigits(2);

  //  TFile *f1 = TFile::Open("out.root");
  TFile *f1 = TFile::Open(filename);
  TCanvas* c1 = new TCanvas("c1","", 1500, 4000);

  const int kNCosThetamuBins = 20;
  const int kNColumns        = 2;
  const int kNEmptyBins      = 4;
  const double kNCosStart = 1.0, kNCosStep = 0.1;
  double CosTheta = kNCosStart;


  //  c1->Divide(kNColumns, (kNCosThetamuBins - kNEmptyBins)/kNColumns , 0.005, 0.001 , 0);
  c1->Divide(kNColumns, (kNCosThetamuBins - kNEmptyBins)/kNColumns , 0.0001, 0.0001, 0);
  //c1->Divide(2, 9, 0.01, 0.01, 0);  
  int choosepad_odd(kNCosThetamuBins-1);
  int choosepad_even(kNCosThetamuBins - (kNCosThetamuBins - 2)/2  );
  
  for(int i = 0; i < (kNCosThetamuBins - kNEmptyBins); i++) {
    
    //cout << "Even, Odd " << choosepad_even << "\t" << choosepad_odd << endl;
    
    int j = kNCosThetamuBins - 1 - i;
    TH1D * histo;
    TH1D * histo_genie;
    // const char * name_data = Form("data_costhetabin_%d",j);	
    // const char * name_genie = Form("genie_costhetabin_%d",j);
    const char * name_data;	    
    const char * name_genie;

    if ( (i+1)%2 == 1 ) {
      name_data = Form("data_costhetabin_%d",choosepad_odd);
      name_genie = Form("genie_costhetabin_%d",choosepad_odd);
      choosepad_odd--;   
    
    } else if ( (i+1)%2 == 0 ) {
      name_data = Form("data_costhetabin_%d",choosepad_even);
      name_genie = Form("genie_costhetabin_%d",choosepad_even);
      choosepad_even--;

    }

    f1->GetObject(name_data,  histo);
    f1->GetObject(name_genie, histo_genie);
   
    // cout << "i%2: " << i%2 << endl;
    histo->SetDirectory(0);
    histo_genie->SetDirectory(0);

    // histo->Scale(1e-41);
    // histo_genie->Scale(1e-41);
    // Simple normalisation of two distributions
    double scale = histo->Integral() / histo_genie->Integral();
    histo_genie->Scale(scale);
 
    // Numbering for pads starts at 1 not 0
    c1->cd(i+1);  
    //gPad->SetLeftMargin(0.2);

    histo_genie->SetLineColor(2);
    //hs->Add(histo_genie);
    
    histo->SetMarkerStyle(20);
    histo->SetMarkerSize(0.75);

    // Get Maximum of histograms
    double histmax = histo->GetMaximum();
    double geniemax = histo_genie->GetMaximum();
    double overallmax = TMath::Max(histmax,geniemax);

    histo_genie->SetMaximum(overallmax*1.3);
    
    histo_genie->GetXaxis()->SetTitle("T_{#mu} (GeV)");
    histo_genie->GetYaxis()->SetTitle("#frac{d^{2} #sigma}{dT_{#mu} d(cos#theta_{#mu}) }(cm^{2}/GeV)");
    //histo_genie->GetXaxis()->SetLabelOffset(0.015);
    histo_genie->GetYaxis()->SetTitleOffset(1.2);
    //histo->GetXaxis()->SetLabelOffset(999);
    //histo->GetXaxis()->SetLabelSize(0);

    gPad->Modified();

    histo_genie->Draw();

    //histo->Draw("PERRSAME");
    //histo->Sumw2();
    histo->Draw("pe1same");

    leg = new TLegend(0.52,0.7,0.9,0.9);
    // Make legend transparent
    leg->SetFillStyle(0);
    // Remove legend box
    leg->SetBorderSize(0);

    const char * coslabel = Form("%.1f < cos #theta_{#mu} < %.1f", (CosTheta - kNCosStep), CosTheta);
        
    std::cout << "CosTheta: " 
	      << CosTheta << "\t" << (CosTheta - kNCosStep) << "\n"
	      << coslabel << std::endl;

    leg->SetHeader(coslabel);
    leg->AddEntry(histo,"CCQE Data","lp");
    leg->AddEntry(histo_genie,"GENIE Prediction","l");
    leg->SetTextSize(0.03);
 
    leg->Draw();

    // t = new TLatex(.5,.5,"TLatex at (.1,.1)");  
    // t->SetNDC(kTRUE);
    // t->Draw();
    CosTheta = CosTheta - kNCosStep;
    double epsilon = 1E-5;
      if (TMath::Abs(CosTheta-kNCosStep) < epsilon) {
	CosTheta = 0.0;
      } 
  }
  c1->Print("c1.pdf","pdf");
  c1->Print("output.ps");

}


