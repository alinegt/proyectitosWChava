#include "SPE_and_timing.h"

/// Constructor
SPE_and_timing::SPE_and_timing()  {}
/// Destructor
SPE_and_timing::~SPE_and_timing() {}


std::string SPE_and_timing::baseName(std::string const &path)
{
  std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
  std::string::size_type const p(base_filename.find_last_of('.'));
  return base_filename.substr(0, p);
}

std::string SPE_and_timing::loadTree(char *argc){
  TFile *f = new TFile(argc);
  T = (TTree *)f->Get("T");
  T->Print();
  rootFileName = baseName(argc);
  return rootFileName;

}


// Function to load the histogram from dat file
TH1F*  SPE_and_timing::loadHistFromFile(double_t limInfBin, double_t limSupBin, double_t numberBin)
{


  TTree *Tcharge = T;

  TH1F *PeakToValleyFit = new TH1F("PeakToValleyFit", "No Coil", numberBin, limInfBin, limSupBin);
  float charge_v;
  ULong64_t nentries = (Int_t)Tcharge->GetEntries();

  Tcharge->SetBranchAddress("charge", &charge_v);

  for (ULong64_t j = 0; j < nentries; j++)
  {
    Tcharge->GetEntry(j);
    PeakToValleyFit->Fill(charge_v);
  }
  Tcharge->ResetBranchAddresses();

  //TCanvas *c1 = new TCanvas();
  //PeakToValleyFit->Draw();
  return PeakToValleyFit;
}
/**
 * @brief Function to fit multi pe spectrum
 * 
 * @param x 
 * @param par      Pointer to parameters for the fit
 * @return Double_t 
 */

Double_t  SPE_and_timing::fitf(Double_t *x, Double_t *par)
{
  Double_t arg = 0;
  //   Double_t arg1 =0;
  Double_t arg2 = 0;
  Double_t arg3 = 0;
  Double_t arg4_1 = 0;
  Double_t arg4 = 0;
  Double_t summing = 0;
  int n = 0;
  Double_t a = par[0];
  Double_t b = par[1];
  Double_t Npe = par[2];
  Double_t sigma = par[3];
  Double_t C = par[4];
  arg = (a * TMath::Exp(-b * x[0]));
  for (n = 0; n < 2; n++)
  {
    arg2 = ((TMath::Power(Npe, n)) * TMath::Exp(Npe)) / TMath::Factorial(n);
    arg3 = TMath::Sqrt(2 * 3.14159 * n * TMath::Power(sigma, 2));
    arg4_1 = x[0] - n * C;
    arg4 = TMath::Exp(-(TMath::Power(arg4_1, 2)) / (2 * n * TMath::Power(sigma, 2)));
    summing += arg2 * arg3 * arg4;
  }

  Double_t fitval = arg + summing;
  summing = 0;
  // Double_t fitval = x[0]*par[0]+par[1] + par[2]+par[3]+ par[4];
  return fitval;
}


/**
 * @brief Main function to get the parameters and generate 
 * histograms
 * 
 * @param argc            Filepath to a root file containing charge and noise branches 
 * @param peak2Valley     Pointer to the peak2Valley value passed to be updated
 * @param sigma_fit       Pointer to the sigma value to get obtained from fit
 * @return Double_t 
 */
Double_t  SPE_and_timing::SPEhistAndPlots(double_t *peak2Valley, double_t *sigma_fit)
{
  Double_t limInfBin = -1;
  Double_t limSupBin = 6;
  Double_t numberBin = 200;
  Double_t adcResolution = (limSupBin - limInfBin) / numberBin;



  TH1F *h_peakToValley = loadHistFromFile(limInfBin, limSupBin, numberBin);
  h_peakToValley->SetMaximum(10e4);
  //h_peakToValley->SetMinimum(0.0);

  //std::string filepath = argc;
  //std::string filename = baseName(filepath);
  
  char *filename_arr = &rootFileName[0];

  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  TPad *pad1 = new TPad("pad1", "", 0, 0.33, 1, 1);
  pad1->SetLogy();
  pad1->SetGrid();
  pad1->Draw();
  pad1->cd();
  Double_t xlimFitPed[2] = {-.1, .125};
  //    Double_t xlimFitPeak2Valley[2]={0.1,2.5};
  h_peakToValley->Draw();
  h_peakToValley->SetLineWidth(2);
  h_peakToValley->SetTitle(Form("%s; Charge [pC] ;Counts", filename_arr));
  h_peakToValley->GetXaxis()->SetTitleSize(.05);
  h_peakToValley->GetYaxis()->SetTitleSize(.05);
  h_peakToValley->GetYaxis()->SetTitleOffset(0.7);
  h_peakToValley->GetXaxis()->SetTitleOffset(1);
  h_peakToValley->GetXaxis()->SetTicks("+-");
  // h_peakToValley->GetXaxis()->SetNdivisions(30);

  ///////////////////////////////////////////////////////////////////////
  ////// FIT Pedestal
  ///////////////////////////////////////////////////////////////////////

  // TF1 *f1 = new TF1("f1", "gaus", -0.3, .2);
  TH1F *h_pedestalFit = (TH1F *)h_peakToValley->Clone("h_pedestalFit");
  h_pedestalFit->Fit("gaus", "Q", "sames", xlimFitPed[0], xlimFitPed[1]);
  h_pedestalFit->GetFunction("gaus")->SetLineColor(kBlack);
  h_pedestalFit->GetFunction("gaus")->SetLineWidth(2);
  TF1 *parGaus = (TF1 *)h_pedestalFit->GetListOfFunctions()->FindObject("gaus");

  *sigma_fit = parGaus->GetParameter(2);
  ///////////////////////////////////////////////////////////////////////
  ////// FIT multiphotoelectron
  ///////////////////////////////////////////////////////////////////////
  // Create a TF1 object using the function defined above.
  // The last three parameters specify the number of parameters
  // for the function.
  TH1F *h_multiph = (TH1F *)h_peakToValley->Clone("MultiPE Fit");
  Double_t limInfFitf = 0.3;
  Double_t limSupFitf = 5;
  Double_t numberOfParams = 5;
  TF1 *funcMulti = new TF1("fitf",SPE_and_timing::fitf, limInfFitf, limSupFitf, numberOfParams);
  // set the parameters to the mean and RMS of the histogram
  funcMulti->SetParameters(445, 0, 3, 1, 1.8);
  // a
  funcMulti->SetParLimits(0, 10, 1000000);
  // b
  funcMulti->SetParLimits(1, 0, 100);
  // Npe
  funcMulti->SetParLimits(2, 0, 3);
  // Sigma
  funcMulti->SetParLimits(3, 0, 5);
  //C
  funcMulti->SetParLimits(4, 1.2, 3);
  // give the parameters meaningful names
  funcMulti->SetParNames("a", "b", "Npe", "sigma", "C");
  // // call TH1::Fit with the name of the TF1 object
  h_multiph->Fit("fitf", "BQr", "sames");
  h_multiph->GetFunction("fitf")->SetLineColor(kBlack);
  h_multiph->GetFunction("fitf")->SetLineWidth(4);

  ///////////////////////////////////////////////////////////////////////
  //get Peak and valley values
  ///////////////////////////////////////////////////////////////////////

  //func->Print();
  Double_t inflectionPoint = 0;
  Double_t xx = 0;
  // The slope will be negative for the first part
  // of the fitted curve, so finding the sign change
  // from negative to positive as an approximation fo the minimum which would be the valley
  for (xx = limInfFitf; xx < limSupFitf; xx += 0.001)
  {
    inflectionPoint = funcMulti->Derivative(xx);
    if (inflectionPoint > 0)
    {
      break;
    }
  }

  // ///////////////////////////////////////////////////////////////////////
  //     // Once the valley was found, the next sign change would be
  //     // a maximum, the peak of the SPE
  // ///////////////////////////////////////////////////////////////////////
  Double_t xx2 = 0;
  for (xx2 = xx; xx2 < limSupFitf; xx2 += 0.001)
  {
    inflectionPoint = funcMulti->Derivative(xx2);
    if (inflectionPoint < 0)
    {
      break;
    }
  }
  // ///////////////////////////////////////////////////////////////////////
  // // Estimating peak/valley
  // ///////////////////////////////////////////////////////////////////////
  Double_t valley = funcMulti->Eval(xx);
  Double_t peak = funcMulti->Eval(xx2);
  *peak2Valley = peak / valley;
  // std::cout << peak2Valley << std::endl;

  ///////////////////////////////////////////////////////////////////////
  ////// FIT SPE 0
  ///////////////////////////////////////////////////////////////////////
  TH1F *h_spe = (TH1F *)h_peakToValley->Clone("SPE 0 Fit");
  //      TF1 *funcMulti = new TF1("gaus",xx,25);
  h_spe->Fit("gaus", "Qr", "sames", xx, 5);

  ///////////////////////////////////////////////////////////////////////
  ////// Stats boxes
  ///////////////////////////////////////////////////////////////////////
  c->Update();
  TPaveStats *ps1 = (TPaveStats *)h_peakToValley->GetListOfFunctions()->FindObject("stats");
  ps1->SetX1NDC(0.5);
  ps1->SetX2NDC(0.9);
  ps1->SetY1NDC(0.75);
  ps1->SetY2NDC(0.8);
  ps1->SetTextSize(.04);
  ps1->SetTextColor(kBlue);
  //ps1->SetOptStat(110);
  ps1->SetOptFit(100);

  ps1->Draw();
  pad1->Modified();
  TPaveStats *ps2 = (TPaveStats *)h_pedestalFit->GetListOfFunctions()->FindObject("stats");
  ps2->SetX1NDC(0.5);
  ps2->SetX2NDC(0.9);
  ps2->SetY1NDC(0.75);
  ps2->SetY2NDC(0.8);
  ps2->SetTextSize(.04);
  ps2->SetTextColor(kBlack);
  ps2->SetOptStat(1000000001);
  ps2->SetOptFit(0001);
  ps2->Draw();

  TPaveStats *ps3 = (TPaveStats *)h_multiph->GetListOfFunctions()->FindObject("stats");
  ps3->SetX1NDC(0.5);
  ps3->SetX2NDC(0.9);
  ps3->SetY1NDC(0.55);
  ps3->SetY2NDC(0.9);
  ps3->SetTextSize(.045);
  ps3->SetTextColor(kBlack);
  ps3->SetOptStat(1000000011);
  ps3->SetOptFit(0001);
  ps3->Draw();

  TPaveStats *ps4 = (TPaveStats *)h_spe->GetListOfFunctions()->FindObject("stats");
  ps4->SetX1NDC(0.5);
  ps4->SetX2NDC(0.9);
  ps4->SetY1NDC(0.35);
  ps4->SetY2NDC(0.55);
  ps4->SetTextSize(.045);
  ps4->SetTextColor(kBlack);
  ps4->SetOptStat(1000000001);
  ps4->SetOptFit(0001);
  ps4->Draw();

  pad1->Modified();

  ///////////////////////////////////////////////////////////////////////
  // LEGENDS
  ///////////////////////////////////////////////////////////////////////
  TLegend *leg = new TLegend(0.65, 0.45, 0.9, 0.55);
  leg->AddEntry(h_peakToValley, filename_arr);
  // leg->AddEntry(a_histogram1, );
  //  leg->Draw();
  ///////////////////////////////////////////////////////////////////////
  /////// Markers for minimum and maximum from fitted polynomial ////////
  ///////////////////////////////////////////////////////////////////////

  //    TMarker *mx = new TMarker(PeaksX[0],PeaksY[0],22);
  //     mx-> SetMarkerSize(2);
  //     mx-> SetMarkerColor(4);
  //  //   mx->Draw("SAME");

  TMarker *mn = new TMarker(xx, valley, 22);
  mn->SetMarkerSize(2.5);
  mn->SetMarkerColor(kGreen);
  mn->Draw("SAME");

  TMarker *mx2 = new TMarker(xx2, peak, 22);
  mx2->SetMarkerSize(2.5);
  mx2->SetMarkerColor(kGreen);
  mx2->Draw("SAME");

  c->Update();

  c->cd();
  ///////////////////////////////////////////////////////////////////////
  ///// Bin content
  ///////////////////////////////////////////////////////////////////////
  TAxis *xaxis = h_multiph->GetXaxis();
  //  TAxis *yaxis = h_multiph->GetYaxis();
  Int_t binxInf = xaxis->FindBin(limInfFitf);
  Int_t binxSup = xaxis->FindBin(limSupBin);

  c->Update();

  Double_t binSum = 0;
  Int_t j = 0;
  for (j = binxInf; j < binxSup; j++)
  {
    binSum += h_multiph->GetBinContent(j);
  }
  Double_t occupancy = 100 * (binSum / (h_multiph->GetEntries()));
  ///////////////////////////////////////////////////////////////////////
  ///// Text box
  ///////////////////////////////////////////////////////////////////////
  TLatex t(0.15, 0.9, Form("Peak/valley:%g", *peak2Valley));
  t.SetTextSize(0.03);
  TLatex t2(0.15, 0.875, Form("Resolution:%g pC/bin", adcResolution));
  t2.SetTextSize(0.03);
  TLatex t3(0.15, 0.85, Form("Occupancy:%g%%", occupancy));
  t3.SetTextSize(0.03);

  t.Draw();
  t2.Draw();
  t3.Draw();

  ///////////////////////////////////////////////////////////////////////
  ///// Residuals plot
  ///////////////////////////////////////////////////////////////////////

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
  pad2->Draw();
  pad2->cd();
  // TF1 *f_SPEGaus = (TF1*) h_spe-> GetListOfFunctions()->FindObject("gaus");
  TF1 *f_SPEGaus = (TF1 *)h_multiph->GetListOfFunctions()->FindObject("fitf");

  TH1F *h_residuals = new TH1F("Residuals Multi SPE Fit", "Residuals MultiSPE Fit", 18, -15, 21);
  //pad1->SetBottomMargin(0.01);
  //pad1->SetBorderMode(0);
  // pad1->SetLogy();
  pad2->SetTopMargin(0.00001);
  pad2->SetBottomMargin(0.25);
  pad2->SetBorderMode(0);
  // pad1->Draw();
  // pad2->Draw();
  pad1->cd();
  h_residuals->SetLineWidth(2);
  h_residuals->SetTitle(Form("SPE fit Residual; Charge [pC] ;Counts"));
  h_residuals->GetXaxis()->SetTitleSize(.11);
  h_residuals->GetYaxis()->SetTitleSize(.11);
  h_residuals->GetYaxis()->SetTitleOffset(0.3);
  h_residuals->GetXaxis()->SetTitleOffset(1);
  h_residuals->GetXaxis()->SetTicks("+-");
  h_residuals->GetXaxis()->SetLabelFont(63);
  h_residuals->GetXaxis()->SetLabelSize(16);
  h_residuals->GetYaxis()->SetLabelFont(63);
  h_residuals->GetYaxis()->SetLabelSize(16);
  h_residuals->GetYaxis()->SetRangeUser(0., 50.);
  pad2->SetGrid();
  pad2->cd();
  double_t res;
  Int_t binxSupGauss = xaxis->FindBin(3.4);
  Int_t binxInfFitf = xaxis->FindBin(limInfFitf);

  // std::cout << h_peakToValley->GetNbinsX()<< std::endl;
  // std::cout << binxInf<< std::endl;
  // std::cout << binxSupGauss<< std::endl;

  for (int i = 0; i < h_multiph->GetNbinsX(); i++)
  {

    if (i >= binxInfFitf && i <= binxSupGauss)
    {
      res = h_multiph->GetBinContent(i) - f_SPEGaus->Eval(h_multiph->GetBinCenter(i));
      h_residuals->Fill(res);
    }
  }

  h_residuals->Draw();

  h_residuals->Fit("gaus", "Q", "sames", -17 + h_residuals->FindFirstBinAbove(0), h_residuals->FindLastBinAbove(0) + 3);
  // std::cout << h_residuals->FindFirstBinAbove(0) << std::endl;
  // std::cout << h_residuals->FindLastBinAbove(0) << std::endl;

  c->Update();

  TPaveStats *psRes = (TPaveStats *)h_residuals->GetListOfFunctions()->FindObject("stats");
  psRes->SetX1NDC(0.6);
  psRes->SetX2NDC(0.9);
  psRes->SetY1NDC(0.5);
  psRes->SetY2NDC(1);
  psRes->SetTextSize(.1);
  // psRes->SetTextColor(kBlack);
  psRes->SetOptStat(1000000001);
  psRes->SetOptFit(0001);
  psRes->Draw();
  // pad2->cd();30
  pad2->Modified();

  c->Update();
  std::string outPath = "./data/plots/";
  c->Print((outPath + rootFileName + ".png").c_str());
  c->Close();
  std::cout << rootFileName << std::endl;

  return 0;
}
/**
 * @brief Generate voltage vs time plots
 * 
 * @param TTree Tree object
 * @param argc Filepath to the root file
 */
void  SPE_and_timing::getTimePlot()
{
  std::string filename = rootFileName;
  char *filename_arr;
  filename_arr = &filename[0];
  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  c->Update();

  TH2F *h2 = new TH2F("h2", Form("%s", filename_arr), 200, 4.5e-7, 5.5e-7, 50, -0.25, 0.1);
  T->Draw("voltage:time>>h2", "", "colz");
  h2->SetStats(0);
  h2->GetZaxis()->SetRangeUser(0., 500.);
  h2->SetTitle(Form("%s; Time [s] ; Amplitude [V]", filename_arr));

  gPad->Update();

  c->SetGrid();
  c->cd();
  std::string outPath = "./data/timePlots/";
  c->Print((outPath + rootFileName + ".png").c_str());
  c->Close();
}

/**
 * @brief  Get the occupancy based on the number of pulses that exceeds
 *  x times the RMS. x ~ 7 in charge estimation script
 * @param TTree   Tree object
 * @param argc    filepath to the root file
 * 
 * @sventurag 
 */
void  SPE_and_timing::PulseThresOccupancy()
{

  std::string filename = rootFileName;
  char *filename_arr;
  filename_arr = &filename[0];
  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  ULong64_t nentries = (Int_t)T->GetEntries();
  T->Draw("noise>>h_noise", "", "goff");
  TH1F *h_noise = (TH1F *)gDirectory->Get("h_noise");
  gPad->Update();
  c->Update();
  float noise_mean = h_noise->GetMean();
  std::cout << noise_mean << std::endl;
  T->Draw("voltage:Iteration$>>Myhist", Form("(Min$(voltage)>-%g)", noise_mean));
  TH1F *h_threshold = (TH1F*)gPad->GetPrimitive("Myhist"); 
  h_threshold->SetTitle(Form("%s; Time [s] ; Amplitude [V]", filename_arr));

  gPad->Update();
  c->Update();

  // h_threshold->Print();
  Double_t h_entries = (Double_t)h_threshold->GetEntries();

  float NoiseWaveformCounts = 100 - (100 * h_entries) / (nentries * 200);
  std::cout << NoiseWaveformCounts << std::endl;
  c->SetGrid();
  c->cd();
  std::string outPath = "./data/occupancy/";
  c->Print((outPath + rootFileName + ".png").c_str());
  c->Close();
}



int main(int argc, char **argv)
{
  Double_t peaktovalley, sigma_fit;
  std::string inFile = argv[1]; // Nombre del archivo
  std::string outFile = argv[2];
  std::string outFilePath = "/home/salvador/github/proyectitosWChava/SPE/data/SPEparam/" + outFile + ".dat";
  std::ofstream a_file;
  
  SPE_and_timing getPlots;
  std::string  fileName;
  fileName =getPlots.loadTree(argv[1]);
  getPlots.SPEhistAndPlots(&peaktovalley, &sigma_fit); //, &occupancy, &sigmaSPE0, &sigmaResSPE0 );
  getPlots.getTimePlot();
  getPlots.PulseThresOccupancy();
  
  a_file.open(outFilePath, std::ios::out | std::fstream::app);
  a_file << fileName << " " << peaktovalley << " " << sigma_fit << " " << std::endl;
  a_file.close();

  std::cout << "END" << std::endl;
  return 0;
}