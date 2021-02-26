#include "SPE_and_timing.h"

/// Constructor
SPE_and_timing::SPE_and_timing()  {}
/// Destructor
SPE_and_timing::~SPE_and_timing() {}


int main(int argc, char **argv)
{
  Double_t peaktovalley, sigma_fit;
  std::string inFile = argv[1]; // Nombre del archivo
  std::string outFile = argv[2]; // output text file
  std::string outputRoot = argv[4]; // Output root file
  std::ofstream a_file;
  // TTree *Tsubset;
  SPE_and_timing getPlots;
  getPlots.noiseMaxIndex = std::stoi(argv[3]); // max index value for noise measurements
  std::string  fileName;
  fileName =getPlots.rootFilename(argv[1], argv[4]);
  getPlots.inputPath = argv[1];
 std::cout<< "getPlots.inputPath" << std::endl;

  std::cout<< getPlots.inputPath << std::endl;
  getPlots.outputPath= argv[4];
  std::cout<< "getPlots.outputPath" << std::endl;

  std::cout<< getPlots.outputPath << std::endl;

  // getPlots.PulseThresOccupancy();
 
   getPlots.sel_pulses();
    // getPlots.plot_sel();
  //  Double_t rms = getPlots.RMSnoise();
  // getPlots.getTimePlot();
  // getPlots.noise();
  std::string outFilePath = ("/home/salvador/github/proyectitosWChava/SPE/data/SPEparam/" + outFile + ".dat").c_str();

  //a_file.open(outFilePath, std::ios::out | std::fstream::app);
  //a_file << fileName << " " << peaktovalley << " " << sigma_fit << " " << std::endl;
  //a_file << fileName << " " << rms << " "  << std::endl;

  //a_file.close();

  std::cout << "END" << std::endl;
  return 0;
}

std::string SPE_and_timing::baseName(std::string const &path)
{
  std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
  std::string::size_type const p(base_filename.find_last_of('.'));
  return base_filename.substr(0, p);
}



std::string SPE_and_timing::rootFilename(char *inputRootPath, char *outputRootPath){
  
  inputRootFileName = baseName(inputRootPath);
  outputRootFileName = baseName(outputRootPath);  
 
  // TFile *f = new TFile(inputPath);
  // TTree *T = (TTree *)f->Get("T");
  // // T->Print();
  // delete T;
  // f->Close();
   return inputRootFileName;

}




// Function to load the histogram from dat file
TH1F*  SPE_and_timing::loadHistFromFile(double_t limInfBin, double_t limSupBin, double_t numberBin)
{

TFile *f = new TFile(inputPath);
  TTree *T = (TTree *)f->Get("T");
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
  delete T;
  f->Close();
 
  //TCanvas *c1 = new TCanvas();
  //PeakToValleyFit->Draw();
  return PeakToValleyFit;
}
/**
 * @brief Function to fit multi pe spectrum.
 * This is a static function, static declaration in header
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
  
  char *filename_arr = &inputRootFileName[0];

  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  TPad *pad1 = new TPad("pad1", "", 0, 0.0, 1, 1);
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
  t.SetTextSize(0.05);
  TLatex t2(0.15, 0.85, Form("Resolution:%g pC/bin", adcResolution));
  t2.SetTextSize(0.05);
  TLatex t3(0.15, 0.8, Form("Occupancy:%g%%", occupancy));
  t3.SetTextSize(0.05);

  t.Draw();
  t2.Draw();
  t3.Draw();

  ///////////////////////////////////////////////////////////////////////
  ///// Residuals plot
  ///////////////////////////////////////////////////////////////////////

  // TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
  // pad2->Draw();
  // pad2->cd();
  // // TF1 *f_SPEGaus = (TF1*) h_spe-> GetListOfFunctions()->FindObject("gaus");
  // TF1 *f_SPEGaus = (TF1 *)h_multiph->GetListOfFunctions()->FindObject("fitf");

  // TH1F *h_residuals = new TH1F("Residuals Multi SPE Fit", "Residuals MultiSPE Fit", 18, -15, 21);
  // //pad1->SetBottomMargin(0.01);
  // //pad1->SetBorderMode(0);
  // // pad1->SetLogy();
  // pad2->SetTopMargin(0.00001);
  // pad2->SetBottomMargin(0.25);
  // pad2->SetBorderMode(0);
  // // pad1->Draw();
  // // pad2->Draw();
  // pad1->cd();
  // h_residuals->SetLineWidth(2);
  // h_residuals->SetTitle(Form("SPE fit Residual; Charge [pC] ;Counts"));
  // h_residuals->GetXaxis()->SetTitleSize(.11);
  // h_residuals->GetYaxis()->SetTitleSize(.11);
  // h_residuals->GetYaxis()->SetTitleOffset(0.3);
  // h_residuals->GetXaxis()->SetTitleOffset(1);
  // h_residuals->GetXaxis()->SetTicks("+-");
  // h_residuals->GetXaxis()->SetLabelFont(63);
  // h_residuals->GetXaxis()->SetLabelSize(16);
  // h_residuals->GetYaxis()->SetLabelFont(63);
  // h_residuals->GetYaxis()->SetLabelSize(16);
  // h_residuals->GetYaxis()->SetRangeUser(0., 50.);
  // pad2->SetGrid();
  // pad2->cd();
  // double_t res;
  // Int_t binxSupGauss = xaxis->FindBin(3.4);
  // Int_t binxInfFitf = xaxis->FindBin(limInfFitf);

  // // std::cout << h_peakToValley->GetNbinsX()<< std::endl;
  // // std::cout << binxInf<< std::endl;
  // // std::cout << binxSupGauss<< std::endl;

  // for (int i = 0; i < h_multiph->GetNbinsX(); i++)
  // {

  //   if (i >= binxInfFitf && i <= binxSupGauss)
  //   {
  //     res = h_multiph->GetBinContent(i) - f_SPEGaus->Eval(h_multiph->GetBinCenter(i));
  //     h_residuals->Fill(res);
  //   }
  // }

  // h_residuals->Draw();

  // h_residuals->Fit("gaus", "Q", "sames", -17 + h_residuals->FindFirstBinAbove(0), h_residuals->FindLastBinAbove(0) + 3);
  // // std::cout << h_residuals->FindFirstBinAbove(0) << std::endl;
  // // std::cout << h_residuals->FindLastBinAbove(0) << std::endl;

  c->Update();

  // TPaveStats *psRes = (TPaveStats *)h_residuals->GetListOfFunctions()->FindObject("stats");
  // psRes->SetX1NDC(0.6);
  // psRes->SetX2NDC(0.9);
  // psRes->SetY1NDC(0.5);
  // psRes->SetY2NDC(1);
  // psRes->SetTextSize(.1);
  // // psRes->SetTextColor(kBlack);
  // psRes->SetOptStat(1000000001);
  // psRes->SetOptFit(0001);
  // psRes->Draw();
  // // pad2->cd();30
  // pad2->Modified();

  c->Update();
  std::string outPath = "./data/plots/";
  c->Print((outPath + inputRootFileName + ".png").c_str());
  c->Close();
  std::cout << inputRootFileName << std::endl;

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
 TFile *f = new TFile(inputPath);
  TTree *T = (TTree *)f->Get("T");
  // T->Print();
  
  std::string filename = inputRootFileName;
  char *filename_arr;
  filename_arr = &filename[0];
  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  c->Update();

  std::string selBr="bvoltage_selected";
  char *selBranch_array;
  selBranch_array = &selBr[0];

  TH2F *h2 = new TH2F("h2", Form("%s", filename_arr), 200, 450, 550, 50, -0.25, 0.1);
  T->Draw(Form( "%s:time/(1e-9)>>h2", selBranch_array ), Form("( ( ( Min$(%s) ) <-%g ) &&  ( ( Max$(%s) ) <%g ) )",selBranch_array ,0.03, selBranch_array,0.1), "colz");
  h2->SetStats(0);
  h2->GetZaxis()->SetRangeUser(0., 500.);
  h2->SetTitle(Form("%s; Time [ns] ; Amplitude [V]", filename_arr));
  h2->GetXaxis()->SetNoExponent();

  gPad->Update();
   TPaletteAxis *palette = (TPaletteAxis*)h2->GetListOfFunctions()->FindObject("palette");

// the following lines move the paletter. Choose the values you need for the position.
   palette->SetX1NDC(0.9);
   palette->SetX2NDC(0.95);
   palette->SetY1NDC(0.2);
   palette->SetY2NDC(0.8);
   gPad->Modified();
   gPad->Update();

  c->SetGrid();
  c->cd();
  std::string outPath = "./data/timePlots/";
  c->Print((outPath + inputRootFileName + ".png").c_str());
  delete T;
  f->Close();
  c->Close();
}


void upd() { TFile *f = new TFile("hs.root","update"); 
TTree *T = (TTree*)f->Get("ntuple"); 
float px,py; float pt; 
TBranch *bpt = T->Branch("pt",&pt,"pt/F"); 
T->SetBranchAddress("px",&px); 
T->SetBranchAddress("py",&py); 
Long64_t nentries = T->GetEntries(); 
for (Long64_t i=0;i<nentries;i++) { 
  T->GetEntry(i); 
  pt = TMath::Sqrt(px*px+py*py); 
  bpt->Fill(); } 
  T->Print(); 
  T->Write(); 
  delete f; }


// void SPE_and_timing::sel_pulses2(){




// }














//Select pulses waveforms


void SPE_and_timing::sel_pulses(){
 std::string filename = outputRootFileName;
  char *filename_arr;
  filename_arr = &filename[0];
  TFile *f = new TFile(inputPath);
   TTree *T = (TTree *)f->Get("T");
// delete f;
    TFile *hfile = new TFile(Form("./data/cutted/%s.root", filename_arr),"RECREATE");
 TTree *Tsel = T->CloneTree();
  Tsel->SetBranchStatus("*", 0);
 
   // Activate only four of them
   for (auto activeBranchName : {"time", "voltage"})
    Tsel->SetBranchStatus(activeBranchName, 1);

// gROOT->cd();
// TTree * Tsubset = new TTree("Tsubset", "Tsubset")
// TTree *Tsubset = T->CloneTree();
std::vector<float> *v_voltage=0;
std::vector<float> *v_voltage_selected=0;
std::vector<float> *v_time=0;
std::vector<float> *v_time2=0;


Tsel->SetBranchAddress("voltage",&v_voltage); 
Tsel->SetBranchAddress("time",&v_time); 


TBranch *bvoltage_selected = Tsel->Branch("bvoltage_selected",&v_voltage_selected); 
TBranch *btime = Tsel->Branch("btime",&v_time2); 

Long64_t nentries = Tsel->GetEntries(); 

for (Long64_t i=0;i<nentries;i++) { 
Tsel->GetEntry(i); 
 auto maxVoltage = std::max_element(v_voltage->begin(),next(v_voltage->begin(), 100) ); 

//auto minVoltageIndex = std::min_element(v_voltage->begin(),next(v_voltage->begin(), 90) ) - v_voltage->begin(); 
 auto minVoltage = std::min_element(v_voltage->begin(),next(v_voltage->begin(), 100) ); 
    
    if ((float)*minVoltage > -0.01){
    if ((float)*maxVoltage < 0.04){

//  if ((int)minVoltageIndex >=0) {
    v_voltage_selected = v_voltage;

    bvoltage_selected->Fill(); 
    btime->Fill();
  //  }
   
 }
    }


} 
Tsel->ResetBranchAddresses();

// T->Write();
// Tsel->Write("",TObject::kOverwrite); // save only the new version of

 Tsel->Print(); 
gROOT->cd();
 
  TCanvas *c2 = new TCanvas("c2", "A3", 1000, 700);
  c2->Update();

  std::string selBr="bvoltage_selected";
  char *selBranch_array;
  selBranch_array = &selBr[0];

  TH2F *h = new TH2F("h", Form("%s", filename_arr), 200, 450, 550, 50, -0.25, 0.1);
  Tsel->Draw(Form( "%s:time/(1e-9)>>h", selBranch_array ),"", "colz");
  h->SetStats(0);
  h->GetZaxis()->SetRangeUser(0., 500.);
  h->SetTitle(Form("%s; Time [ns] ; Amplitude [V]", filename_arr));
  h->GetXaxis()->SetNoExponent();
    gPad->Update();

 c2->SetGrid();
  c2->cd();
  std::string outPath = "./data/timePlots/";
  c2->Print((outPath + outputRootFileName + ".png").c_str());

  c2->Close();

 hfile->Write();
 hfile->Close();
 
  

}


void SPE_and_timing::plot_sel(){
  
   std::string filename = outputRootFileName;
  char *filename_arr;
  filename_arr = &filename[0];

  // std::cout<< outputPath <<endl;
    TFile *hfile = new TFile(Form("./data/cutted/%s.root", filename_arr));
  TTree *T = (TTree *)hfile->Get("T");
  T->Print();


  TCanvas *c2 = new TCanvas("c2", "A3", 1000, 700);
  // c2->Update();

  // std::string selBr="bvoltage_selected";
  // char *selBranch_array;
  // selBranch_array = &selBr[0];

  TH2F *h2 = new TH2F("h2", "h2", 200, 450, 550, 50, -0.25, 0.1);
  T->Draw("bvoltage_selected:time/(1e-9)>>h2","", "colz");
  // TH1F *h =new TH1F("h", "h", 200, 0.0, 0.05);
  // T->Draw("brms>>h");
  // T->Draw("bvoltage_selected:time/(1e-9)>>htemp","", "colz");
  // h->SetStats(0);
 h2->GetZaxis()->SetRangeUser(0., 500.);
  // h->SetTitle(Form("%s; Time [ns] ; Amplitude [V]", filename_arr));
//   h->GetXaxis()->SetNoExponent();
//     gPad->Update();
   c2->Update();

//  c2->SetGrid();
//   c2->cd();
  std::string outPath = "./data/timePlots/";
  c2->Print((outPath + outputRootFileName + ".png").c_str());

   c2->Close();
  //  Tsubset->Write(); 
// 
  //  delete T;
    // T->Print(); 
  // Tsel->Write(); 
  // f->Write();
   hfile->Close();
  // delete Tsel;


  // hfile->Write();  
  // hfile->Close();

}


float SPE_and_timing::rms(vector <float> *v_voltage){
float sum=0, rms=0;
  for (auto it = v_voltage->begin() ; it != v_voltage->begin()+noiseMaxIndex; ++it){
sum += pow(*it, 2);
    } 
rms = sqrt(sum/noiseMaxIndex);
return rms;


}



// Return std of gaussian fit for a vector 
Double_t SPE_and_timing::h_std(vector <float> *v_voltage){
 TCanvas *c3 = new TCanvas("c3", "A3", 1000, 700);
  c3->Update();

  // auto numberBin=  noise.size();
  // auto limInfBin=  noise.begin();
  // auto limSupBin=  noise.end();
  TH1F *h_noiseFit = new TH1F("h_noiseFit", "Noise Hist", 25, -0.05, 0.05);
  for (auto it = v_voltage->begin() ; it != v_voltage->begin()+noiseMaxIndex; ++it)
  {
    h_noiseFit->Fill(*it);
  }
  h_noiseFit->Draw();

  h_noiseFit->Fit("gaus", "Q", "sames");
  h_noiseFit->GetFunction("gaus")->SetLineColor(kBlack);
  h_noiseFit->GetFunction("gaus")->SetLineWidth(2);
  TF1 *parGaus = (TF1 *)h_noiseFit->GetListOfFunctions()->FindObject("gaus");
  
  Double_t noiseSigmaFit = 0;
  noiseSigmaFit = parGaus->GetParameter(2);
  
  gPad->Update();
 c3->SetGrid();
  c3->cd();
  // std::string outPath = "./data/NoiseFits/";
  // int randomN;
  // randomN = rand() % 50000 + 1;
  // std::string randomNstring = std::to_string(randomN);
  //c3->Print((outPath + rootFileName + randomNstring +".png").c_str());
  
  
  delete c3;
  delete h_noiseFit;
  
  return noiseSigmaFit;
}




//Noise measurements

// void SPE_and_timing::noise(){
// TTree *Tnoise = Tsubset;
// Tnoise->Print(); 


// }





Double_t SPE_and_timing::RMSnoise(){
  
  TFile *f = new TFile(outputPath);
  TTree *T = (TTree *)f->Get("T");
  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  TH1F *h_noise_rms = new TH1F("h_noise_rms", "rms noise",50,0.00,0.01 );
 T->Draw("noise>>h_noise_rms");

  h_noise_rms->Fit("gaus", "Q", "sames");
  h_noise_rms->GetFunction("gaus")->SetLineColor(kRed);
  h_noise_rms->GetFunction("gaus")->SetLineWidth(2);
  // TH1F *h_noise = (TH1F *)gDirectory->Get("h_noise");
  gPad->Update();
  c->Update();
  Double_t noise_rms_mean = h_noise_rms->GetMean();
  std::cout << "noise_rms_mean" << std::endl;
   h_noise_rms->SetLineColor(kRed);

  std::cout << noise_rms_mean << std::endl;
  //noise_mean = 0.03;

  TH1F *h_noise_std = new TH1F("h_noise_std", "std_noise",50,0.0,0.01 );
   h_noise_std->SetLineColor(kBlack);

  T->Draw("noise_std>>h_noise_std","", "sames");

  h_noise_std->Fit("gaus", "Q", "sames");
  h_noise_std->GetFunction("gaus")->SetLineColor(kBlack);
  h_noise_std->GetFunction("gaus")->SetLineWidth(2);
  gPad->Update();
  c->Update();
  

  Double_t noise_std_mean = h_noise_std->GetMean();
  std::cout << "noise_std_mean" << std::endl;

  std::cout << noise_std_mean << std::endl;

 TH1F *h_noise_rms_sel = new TH1F("h_noise_rms_sel", "rms sel",50,0.0,0.01 );
  T->Draw("brms>>h_noise_rms_sel","", "sames");
     h_noise_rms_sel->SetLineColor(kBlue);
h_noise_rms_sel->Fit("gaus", "Q", "sames");
  h_noise_rms_sel->GetFunction("gaus")->SetLineColor(kBlue);
  h_noise_rms_sel->GetFunction("gaus")->SetLineWidth(2);
  gPad->Update();
  c->Update();
  
  Double_t noise_rms_sel_mean = h_noise_rms_sel->GetMean();
  std::cout << "noise_rms_sel_mean" << std::endl;

  std::cout << noise_rms_sel_mean << std::endl;


  std::string outPath = "./data/NoisePlots/";
  int randomN;
  //randomN = rand() % 50000 + 1;
  std::string randomNstring = std::to_string(randomN);
  c->Print((outPath + inputRootFileName +".png").c_str());
  
  delete T;
  f->Close();
  // return noise_rms_mean;
  return noise_rms_sel_mean;
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

TFile *f = new TFile(inputPath);
// delete T;
  TTree *T = (TTree *)f->Get("T");
  std::string filename = inputRootFileName;
  char *filename_arr;
  filename_arr = &filename[0];
  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  ULong64_t nentries = (Int_t)T->GetEntries();
  // T->Draw("noise>>h_noise");
  // TH1F *h_noise = (TH1F *)gDirectory->Get("h_noise");
  // gPad->Update();
  // c->Update();
  // float noise_mean = h_noise->GetMean();
  std::cout << noise_mean << std::endl;
// This part of the code is because when using gDirectory->Get... GenEntries is always zero,
// and when using gPad->GetPrimitive, SetTitle does not work. So, I have to draw the tree twice
  T->Draw("voltage:Iteration$>>h_sel_entries", Form("(Min$(voltage)>-%g)", noise_mean));
 TH1F *h_sel_entries = (TH1F*)gDirectory->Get("h_sel_entries");               
  auto h_entries= h_sel_entries->GetEntries();

  T->Draw("voltage:Iteration$>>Myhist", Form("(Min$(voltage)<-%g)", noise_mean));
 // TH1F *h_threshold = (TH1F*)gDirectory->Get("Myhist");
  TH1F *h_threshold = (TH1F*)gPad->GetPrimitive("Myhist"); 
//
  h_threshold->SetTitle(Form("%s; Time [s] ; Amplitude [V]", filename_arr));


  // h_threshold->Print();
    std::cout <<  "No. Entries"  << std::endl;

  std::cout << nentries -h_entries/200  << std::endl;

  float NoiseWaveformCounts = 100 - (100 * h_entries) / (nentries * 200);
  std::cout << NoiseWaveformCounts << std::endl;
   gPad->Update();
  c->Update();
  c->SetGrid();
  c->cd();
  std::string outPath = "./data/occupancy/";
  c->Print((outPath + inputRootFileName + ".png").c_str());
  // delete T;
  f->Close();
  c->Close();
}


