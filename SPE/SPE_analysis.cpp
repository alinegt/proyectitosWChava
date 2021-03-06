#include "SPE_analysis.h"

/// Constructor
SPE_analysis::SPE_analysis() {}
/// Destructor
SPE_analysis::~SPE_analysis() {}

/**
 * @brief Get a Tree from a ROOT file containing the pulse waveforms
 * and perform the SPE analysis.
 *  
 * @param argv1 Path to a root file
 * @param argv2 Output text file name
 * @param argv3 Value of the max index for noise calculations
 * @param argv4 Output Root file name
    
 * @return int 
 */
int main(int argc, char **argv)
{
  Double_t peak2valley, mean_fit_ped ,sigma_fit_ped, mean_fit_spe ,sigma_fit_spe;
  std::ofstream a_file;
  Double_t rms_noise, occupancy_bynoise, rms_noise_sigma;
  // Double_t std_noise;

  SPE_analysis getPlots;

  getPlots.inputPath = argv[1];
  std::cout << "getPlots.inputPath" << std::endl;
  getPlots.param_out_file = argv[2];
  std::cout << getPlots.param_out_file << std::endl;
  getPlots.noiseMaxIndex = std::stoi(argv[3]); // max index value for noise measurements
  getPlots.outputPath = argv[4];
  getPlots.sel_condition = argv[5];
  //getPlots.sampleNumber=std::stoi(argv[6]);
  std::cout << "getPlots.outputPath" << std::endl;
  std::cout << getPlots.outputPath << std::endl;

  // std::string fileName;
  std::string fileName = getPlots.rootFilename(argv[1], argv[4]);

  getPlots.getXaxisTime();

  getPlots.RMSnoise(&rms_noise, &rms_noise_sigma);

  std::cout << "selecting pulses" << std::endl;
  getPlots.sel_pulses(&rms_noise, &occupancy_bynoise);

  std::cout << "Plotting SPE" << std::endl;

  getPlots.SPEhistAndPlots(&peak2valley, &mean_fit_ped , &sigma_fit_ped, &mean_fit_spe ,&sigma_fit_spe, &occupancy_bynoise);


  // Writting param to file
  std::string outFilePath = ("/home/idlab-52/github/proyectitosWChava/SPE/data/SPEparam/" + (std::string)getPlots.param_out_file + ".dat").c_str();
  a_file.open(outFilePath, std::ios::out | std::fstream::app);
 // a_file << fileName << " " << peaktovalley << " " << sigma_fit << " " << std::endl;
  a_file << fileName << " " << rms_noise << " " << rms_noise_sigma << " " <<mean_fit_ped << " " <<sigma_fit_ped << " "<< mean_fit_spe << " " <<sigma_fit_spe << " " <<occupancy_bynoise <<std::endl;

  a_file.close();

  std::cout << "END" << std::endl;
  return 0;
}

/**
 * @brief Get the name of a directory
 * 
 * @param path Path to the directory
 * @return std::string 
 */
std::string SPE_analysis::baseName(std::string const &path)
{
  std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
  std::string::size_type const p(base_filename.find_last_of('.'));
  return base_filename.substr(0, p);
}

/**
 * @brief Get the name of output/input Root file
 * 
 * @param inputRootPath  Path to the input ROOT file
 * @param outputRootPath Path to the ouput ROOT file
 * @return std::string 
 */
std::string SPE_analysis::rootFilename(char *inputRootPath, char *outputRootPath)
{

  inputRootFileName = baseName(inputRootPath);
  outputRootFileName = baseName(outputRootPath);

  // TFile *f = new TFile(inputPath);
  // TTree *T = (TTree *)f->Get("T");
  // // T->Print();
  // delete T;
  // f->Close();
  return inputRootFileName;
}

/**
 * @brief Get the limits in the x axis from time values
 * 
 */
void SPE_analysis::getXaxisTime()
{
  // TFile *f0 = TFile::Open("noCoil2.root", "read");
  TFile *f0 = TFile::Open(inputPath, "read");
  TTree *T = (TTree *)f0->Get("T");

  TVectorD *dataParams = (TVectorD *)T->GetUserInfo()->At(0);
  noMuestras = dataParams[0][0] - 1;
  noWaveforms = dataParams[0][1];
  deltaTime = dataParams[0][2];

  TTreeReader aReader("T", f0);
  TTreeReaderValue<std::vector<float>> timeRV(aReader, "time");
  unsigned int it = 0;
  while (aReader.Next())
  {
    for (auto &&value : *timeRV)
    {
      if (it == 0)
      {
        start_time = round(value * 1e9);
        cout << value << endl;
      }

      if (it == (noMuestras))
      {
        end_time = round(value * 1e9);
        cout << value << endl;
        break;
      }
      it++;
    }
    it = 0;

    break;
  }
  delete f0;
}

/**
 * @brief Create an histogram from a branch in the input ROOT file
 * 
 * @param limInfBin Histogram parameter
 * @param limSupBin Histogram parameter //used for occupancy estimation
 * @param numberBin Histogram parameter
 * @return TH1F* 
 */
TH1F *SPE_analysis::loadHistFromFile(double_t limInfBin, double_t limSupBin, double_t numberBin)
{

  std::string filename = outputRootFileName;
  char *filename_arr;
  filename_arr = &filename[0];

  TFile *f_loadhist = new TFile(Form("./data/cutRootFiles/%s/%s.root",param_out_file, filename_arr), "read");
  // TFile *f_loadhist = new TFile(inputPath);
  TTree *T = (TTree *)f_loadhist->Get("T");
  TTree *Tcharge = T;

  TH1F *PeakToValleyFit = new TH1F("PeakToValleyFit", "No Coil", numberBin, limInfBin, limSupBin);
  float charge_v;
  //Bool_t noOutlier_b;
  // Double_t start_time;
  // Double_t end_time;
  std::vector<float> *time_v = 0;

  ULong64_t nentries = (Int_t)Tcharge->GetEntries();

  Tcharge->SetBranchAddress("charge", &charge_v);
  Tcharge->SetBranchAddress("time", &time_v);
  //Tcharge->SetBranchAddress("noOutlier",&noOutlier_b);

  for (ULong64_t j = 0; j < nentries; j++)
  {
    Tcharge->GetEntry(j);
    //   if (noOutlier_b==1){
    PeakToValleyFit->Fill(charge_v);
    //  }

    // if (j==0){

    // start_time = round(time_v->at(0)*1e9);
    // end_time = round(time_v->at( time_v->size()-1 )*1e9);

    // // start_time_int = (  start_time*1e9 );
    // // end_time_int = ( end_time*1e9 );
    // std::cout<< start_time <<std::endl;
    // std::cout<< end_time <<std::endl;

    // }
  }

  Tcharge->ResetBranchAddresses();

  return PeakToValleyFit;
}

/**
 * @brief Function to fit multi pe spectrum.
 * This is a static function, static declaration in header
 * 
 * @param x Tcharge->SetBranchAddress("charge", &charge_v);
 * @param par      Pointer to parameters for the fit
 * @return Double_t 
 */
Double_t SPE_analysis::fitf(Double_t *x, Double_t *par)
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
Double_t SPE_analysis::SPEhistAndPlots(double_t *peak2Valley, double_t *mean_fit_ped ,double_t *sigma_fit_ped ,double_t *mean_fit_spe, double_t *sigma_fit_spe, double_t *occupancy_bynoise)
{
  Double_t limInfBin = -2;
  Double_t limSupBin = 10;
  Double_t numberBin = 200;
  Double_t adcResolution = (limSupBin - limInfBin) / numberBin;

  TH1F *h_peakToValley = loadHistFromFile(limInfBin, limSupBin, numberBin);
   h_peakToValley->SetMaximum(10e5);
  //h_peakToValley->SetMinimum(0.0);

  //std::string filepath = argc;
  //std::string filename = baseName(filepath);

  char *filename_arr = &inputRootFileName[0];

  TCanvas *c_hist = new TCanvas("c_hist", "A3", 1000, 700);
  TPad *pad1 = new TPad("pad1", "", 0, 0.0, 1, 1);
  pad1->SetLogy();
  pad1->SetGrid();
  pad1->Draw();
  pad1->cd();
  Double_t xlimFitPed[2] = {-0.2, 0.2};
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
  TH1F *h_pedestalFit = (TH1F *)h_peakToValley->Clone("Pedestal fit");
  h_pedestalFit->Fit("gaus", "Q", "sames", xlimFitPed[0], xlimFitPed[1]);
  h_pedestalFit->GetFunction("gaus")->SetLineColor(kBlack);
  h_pedestalFit->GetFunction("gaus")->SetLineWidth(2);
  TF1 *parGaus_ped = (TF1 *)h_pedestalFit->GetListOfFunctions()->FindObject("gaus");
  *mean_fit_ped =  parGaus_ped->GetParameter(1);
  *sigma_fit_ped = parGaus_ped->GetParameter(2);
  
  ///////////////////////////////////////////////////////////////////////
  ////// FIT multiphotoelectron
  ///////////////////////////////////////////////////////////////////////
  // Create a TF1 object using the function defined above.
  // The last three parameters specify the number of parameters
  // for the function.
  TH1F *h_multiph = (TH1F *)h_peakToValley->Clone("MultiPE Fit");
  Double_t limInfFitf = 0.3;
  Double_t limSupFitf = 10;
  Double_t numberOfParams = 5;
  TF1 *funcMulti = new TF1("fitf", SPE_analysis::fitf, limInfFitf, limSupFitf, numberOfParams);
  // set the parameters to the mean and RMS of the histogram
  funcMulti->SetParameters(445, 0, 3, 1, 1.8);
  // a
  funcMulti->SetParLimits(0, -100, 10000000);
  // b
  funcMulti->SetParLimits(1, 0, 300);
  // Npe
  funcMulti->SetParLimits(2, 0, 5);
  // Sigma
  funcMulti->SetParLimits(3, 0, 5);
  //C
  funcMulti->SetParLimits(4, 1.2, 4);
  // give the parameters meaningful names
  funcMulti->SetParNames("a", "b", "Npe", "sigma", "C");
  // // call TH1::Fit with the name of the TF1 object
  h_multiph->Fit("fitf", "BQr0", "sames");
  // h_multiph->GetFunction("fitf")->SetLineColor(kBlack);
  // h_multiph->GetFunction("fitf")->SetLineWidth(4);

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
  std::cout << "all OK" << std::endl;
  ///////////////////////////////////////////////////////////////////////
  ////// FIT SPE 0
  ///////////////////////////////////////////////////////////////////////
  TH1F *h_spe = (TH1F *)h_peakToValley->Clone("SPE 0 Fit");
    //    TF1 *funcMulti = new TF1("gaus",xx,25);
    h_spe->Fit("gaus", "Qr", "sames", xx+0.3, 5);
  TF1 *parGaus_spe = (TF1 *)h_spe->GetListOfFunctions()->FindObject("gaus");
  
  *mean_fit_spe =  parGaus_spe->GetParameter(1);
  *sigma_fit_spe =  parGaus_spe->GetParameter(2);

  ///////////////////////////////////////////////////////////////////////
  ////// Stats boxes
  ///////////////////////////////////////////////////////////////////////
  c_hist->Update();
  TPaveStats *ps1 = (TPaveStats *)h_peakToValley->GetListOfFunctions()->FindObject("stats");
  ps1->SetX1NDC(0.6);
  ps1->SetX2NDC(0.9);
  ps1->SetY1NDC(0.75);
  ps1->SetY2NDC(0.8);
  ps1->SetTextSize(.025);
  ps1->SetTextColor(kBlue);
  ps1->SetOptStat(11);
  ps1->SetOptFit(111);

  ps1->Draw();
  pad1->Modified();
  TPaveStats *ps2 = (TPaveStats *)h_pedestalFit->GetListOfFunctions()->FindObject("stats");
  ps2->SetX1NDC(0.55);
  ps2->SetX2NDC(0.9);
  ps2->SetY1NDC(0.65);
  ps2->SetY2NDC(0.9);
  ps2->SetTextSize(.03);
  ps2->SetTextColor(kBlack);
  ps2->SetOptStat(1000000001);
  ps2->SetOptFit(110);
  TList *listOfLines = ps2->GetListOfLines();
   // Remove the RMS line
   TText *tconst = ps2->GetLineWith("ENTRIES");
   listOfLines->Remove(tconst);


  ps2->Draw();

  // TPaveStats *ps3 = (TPaveStats *)h_multiph->GetListOfFunctions()->FindObject("stats");
  // ps3->SetX1NDC(0.6);
  // ps3->SetX2NDC(0.9);
  // ps3->SetY1NDC(0.55);
  // ps3->SetY2NDC(0.9);
  // ps3->SetTextSize(.045);
  // ps3->SetTextColor(kBlack);
  // ps3->SetOptStat(1000000011);
  // ps3->SetOptFit(0001);
 // ps3->Draw();

  TPaveStats *ps4 = (TPaveStats *)h_spe->GetListOfFunctions()->FindObject("stats");
  ps4->SetX1NDC(0.55);
  ps4->SetX2NDC(0.9);
  ps4->SetY1NDC(0.45);
  ps4->SetY2NDC(0.65);
  ps4->SetTextSize(.03);
  ps4->SetTextColor(kRed);
  ps4->SetOptStat(1000000001);
  ps4->SetOptFit(112);
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

  c_hist->Update();

  c_hist->cd();
  ///////////////////////////////////////////////////////////////////////
  ///// Bin content
  ///////////////////////////////////////////////////////////////////////
  TAxis *xaxis = h_multiph->GetXaxis();
  //  TAxis *yaxis = h_multiph->GetYaxis();
  Int_t binxInf = xaxis->FindBin(limInfFitf);
  Int_t binxSup = xaxis->FindBin(limSupBin);

  c_hist->Update();

  Double_t binSum = 0;
  Int_t j = 0;
  for (j = binxInf; j < binxSup; j++)
  {
    binSum += h_multiph->GetBinContent(j);
  }
  Double_t occupancy = 100 * (binSum / (h_multiph->GetEntries())); // THI IS NOT VERY ACCURATE
  ///////////////////////////////////////////////////////////////////////
  ///// Text box
  ///////////////////////////////////////////////////////////////////////
  TLatex t(0.15, 0.8, Form("Peak/valley:%g", *peak2Valley));
  t.SetTextSize(0.03);
  TLatex t2(0.15, 0.75, Form("Resolution:%g pC/bin", adcResolution));
  t2.SetTextSize(0.05);
  // TLatex t3(0.15, 0.8, Form("Occupancy:%g%%", occupancy)); // THIS APPROACH IS BASED ON SPE plot, not very accurate
  TLatex t3(0.15, 0.75, Form("Occupancy:%g%%", *occupancy_bynoise*100)); //THIS APPROACH IS BASED ON noise level thresholds

  t3.SetTextSize(0.03);

  t.Draw();
  // t2.Draw();
   t3.Draw();

  c_hist->Update();
  std::string outPath = "./data/SPEplots/";
  c_hist->Print((outPath + param_out_file + "/" + outputRootFileName + ".png").c_str());
  c_hist->Close();
  std::cout << inputRootFileName << std::endl;

  return 0;
}

/**
 * @brief Select waveforms
 * 
 */
void SPE_analysis::sel_pulses(Double_t *rms_noise, Double_t *occupancy_bynoise)
{
  int nthreads = 4;
  ROOT::EnableImplicitMT(nthreads);

  std::string filename = outputRootFileName;
  char *filename_arr;
  filename_arr = &filename[0];

  // Open input file containing a Tree
  TFile *f = new TFile(inputPath);
  TTree *T = (TTree *)f->Get("T");

  // Create a new file and cloning the input Tree into the output Tree
  TFile *hfile = new TFile(Form("./data/cutRootFiles/%s/%s.root", param_out_file ,filename_arr), "RECREATE");
  TTree *Tsel = T->CloneTree();

  // Variable from the input Tree
  std::vector<float> *v_voltage = 0;
  // New Variable for the output Tree
  Bool_t bol_noOutliers;
  Bool_t bol_noise_sel;

  Float_t f_rms;
  Float_t f_std;
  Bool_t bol_sel_pulse;

  // Set branch Address for the input Tree variable
  Tsel->SetBranchAddress("voltage", &v_voltage);

  // Create new branch
  TBranch *noOutliers = Tsel->Branch("noOutliers", &bol_noOutliers);
  TBranch *brms = Tsel->Branch("brms", &f_rms);
  TBranch *noise_sel = Tsel->Branch("noise_sel", &bol_noise_sel);
  TBranch *pulses = Tsel->Branch("pulses", &bol_sel_pulse);
  int noiseFactorOutliers = 12;
  int noiseFactorPulse = 10;

  // For loop to fill NEW BRANCH
  Long64_t nentries = Tsel->GetEntries();
  for (Long64_t i = 0; i < nentries; i++)
  {
    Tsel->GetEntry(i);
    /*   
    //Base line noise
    auto minVoltageNoise = std::min_element(v_voltage->begin(), next(v_voltage->begin(), noiseMaxIndex));
 */
    //Base line noise
    // auto maxVoltagePulse = std::max_element(v_voltage->begin(), next(v_voltage->begin(), 199));
    auto minVoltage = std::min_element(v_voltage->begin(), next(v_voltage->begin(), (noMuestras)));
    auto maxVoltage = std::max_element(v_voltage->begin(), next(v_voltage->begin(), noMuestras));

    if (((float)*minVoltage > *rms_noise * (noiseFactorOutliers * (-1))) & ((float)*maxVoltage < *rms_noise * noiseFactorOutliers))
    {

      bol_noise_sel = true;
    }
    else
    {
      bol_noise_sel = false;
    }

    if ((float)*maxVoltage < *rms_noise * noiseFactorOutliers)
    {

      bol_noOutliers = true;
    }
    else
    {
      bol_noOutliers = false;
    }

    if ((float)*minVoltage < *rms_noise * (noiseFactorPulse * (-1)))
    {
      bol_sel_pulse = true;
    }
    else
    {
      bol_sel_pulse = false;
    }

    Float_t sum = 0;
    for (int i = 0; i < noiseMaxIndex; i++)
    {
      sum += pow(v_voltage->at(i), 2);
    }
    f_rms = sqrt(sum / noiseMaxIndex);

    // f_std=h_std(v_voltage);
    //  std::cout<< f_std<<std::endl;

    noOutliers->Fill(); // NOTE : Filling new branch
    brms->Fill();
    noise_sel->Fill();
    pulses->Fill();
  }
  Tsel->ResetBranchAddresses();

  TCanvas *c2 = new TCanvas("c2", "A3", 1000, 700);
  c2->Update();

  std::string selBr = "voltage";
  char *selBranch_array;
  selBranch_array = &selBr[0];
  Double_t xnbins = noMuestras;
  Double_t ynbins = 500;

  Double_t ylow = -0.5;
  Double_t yhigh = 0.1;

  //char const *selection="pulses==1";
  TCut sel;
  if (sel_condition == NULL)
  {
    sel = "";
  }
  else
  {
    sel = sel_condition;
  }

  std::cout << "Time plots" << endl;

  TH2F *h = new TH2F("h", Form("%s", filename_arr), xnbins, start_time, end_time, ynbins, ylow, yhigh);
  Tsel->Draw(Form("%s:time/(1e-9)>>h", selBranch_array), sel, "colz");
  h->SetStats(0);
  h->GetZaxis()->SetRangeUser(0., 200.);
  h->SetTitle(Form("%s; Time [ns] ; Amplitude [V]", filename_arr));
  h->GetXaxis()->SetNoExponent();
  h->Write();

  std::cout << "Getting occupancy" << endl;

  int Npulses = Tsel->Draw("pulses>>histPulses", "pulses==1", "goff");
  // int NnoOutliers = Tsel->Draw("noOutlier>>histPulses","noOutlier==1","goff");

  std::cout << Npulses << endl;
  *occupancy_bynoise = (Double_t)Npulses / (Double_t)noWaveforms;
  
  std::cout << "occupancy by noise" << endl;

  std::cout << *occupancy_bynoise << endl;

  gPad->Update();

  c2->SetGrid();
  gStyle->SetGridWidth(3);
  c2->cd();
  std::string outPath = "./data/timePlots/";
  c2->Print((outPath + param_out_file + "/" + outputRootFileName + ".png").c_str());

  c2->Close();

  hfile->Write();
  hfile->Close();
  ROOT::DisableImplicitMT();

  delete hfile;
}

//
/**
 * @brief Return std of gaussian fit for a vector
 * 
 * @param v_voltage Vector
 * @return Float_t 
 */
Float_t SPE_analysis::h_std(vector<float> *v_voltage)
{
  TCanvas *c3 = new TCanvas("c3", "A3", 1000, 700);
  c3->Update();

  // auto numberBin=  noise.size();
  // auto limInfBin=  noise.begin();
  // auto limSupBin=  noise.end();
  TH1F *h_noiseFit = new TH1F("h_noiseFit", "Noise Hist", 25, -0.05, 0.05);
  for (auto it = v_voltage->begin(); it != v_voltage->begin() + noiseMaxIndex; ++it)
  {
    h_noiseFit->Fill(*it);
  }
  h_noiseFit->Draw();

  h_noiseFit->Fit("gaus", "Q", "sames");
  h_noiseFit->GetFunction("gaus")->SetLineColor(kBlack);
  h_noiseFit->GetFunction("gaus")->SetLineWidth(2);
  TF1 *parGaus = (TF1 *)h_noiseFit->GetListOfFunctions()->FindObject("gaus");

  Float_t noiseSigmaFit = 0;
  noiseSigmaFit = parGaus->GetParameter(2);

  gPad->Update();
  c3->SetGrid();
  c3->cd();
  std::string outPath = "./data/NoiseFits/";
  int randomN;
  randomN = rand() % 10 + 1;
  std::string randomNstring = std::to_string(randomN);
  // c3->Print((outPath + outputRootFileName + randomNstring +".png").c_str());

  delete c3;
  delete h_noiseFit;

  return noiseSigmaFit;
}

/**
 * @brief Get rms noise with two different methods
 * 
 * @param rms_noise Pointer to a double to be updated
 * @return Double_t 
 */
Double_t SPE_analysis::RMSnoise(Double_t *rms_noise, Double_t *rms_noise_sigma)
{

  // Open input file containing a Tree
  TFile *f = new TFile(inputPath, "read");
  TTree *T = (TTree *)f->Get("T");
  char *filename_arr = &inputRootFileName[0];

  TCanvas *c = new TCanvas("c", "A3", 1000, 700);
  TH1F *h_noise_rms = new TH1F("h_noise_rms", "rms noise", 300, 0., 0.01);
  T->Draw("noise>>h_noise_rms");
  h_noise_rms->SetTitle(Form("%s; RMS noise ;Counts", filename_arr));

  h_noise_rms->Fit("gaus", "Q", "sames", 0.0024, 0.0041);
  h_noise_rms->GetFunction("gaus")->SetLineColor(kRed);
  h_noise_rms->GetFunction("gaus")->SetLineWidth(2);
  // TH1F *h_noise = (TH1F *)gDirectory->Get("h_noise");
  gPad->Update();
  c->Update();
  // Double_t noise_rms_mean = h_noise_rms->GetMean();
  Double_t noise_rms_mean = h_noise_rms->GetFunction("gaus")->GetParameter(1);
  Double_t noise_rms_sigma = h_noise_rms->GetFunction("gaus")->GetParameter(2);

  TPaveStats *ps = (TPaveStats *)h_noise_rms->GetListOfFunctions()->FindObject("stats");
  ps->SetX1NDC(0.5);
  ps->SetX2NDC(0.9);
  ps->SetY1NDC(0.6);
  ps->SetY2NDC(0.9);

  ps->SetTextSize(.035);
  ps->SetOptStat(1000000011);
  // ps->SetOptStat(11);
  // ps->SetTextColor(kBlue);
  ps->SetOptFit(1111);
  h_noise_rms->GetYaxis()->SetRangeUser(0., 6000.);

  

  c->SetGrid();
  c->Update();
  gPad->Update();
  

  std::cout << "noise_rms_mean" << std::endl;
  // h_noise_rms->SetLineColor(kRed);

  *rms_noise = noise_rms_mean;
  *rms_noise_sigma = noise_rms_sigma;
  
  std::cout << noise_rms_mean << std::endl;
  std::string outPath = "./data/NoisePlots/";
  c->Print((outPath + param_out_file + "/" + outputRootFileName + ".png").c_str());


  delete T;
  f->Close();
  // return noise_rms_mean;
  return noise_rms_mean;
}
