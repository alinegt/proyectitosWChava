#ifndef _SPE_AND_TIMING_H
#define _SPE_AND_TIMING_H




#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
// #include <numeric>


#include "TROOT.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TSpectrum.h"
#include "TMarker.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TString.h"
#include "TGraph.h"
#include <stdlib.h>
#include "TMath.h"
#include "TF1.h"
#include "TGraph2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaletteAxis.h"
#include "TTreeReader.h"
#include "TCut.h"
#include "TVectorD.h"

using namespace std;


class SPE_and_timing{
  public:
SPE_and_timing();
~SPE_and_timing();

Double_t RMSnoise(Double_t *rms_noise);
std::string baseName(std::string const &path);
TH1F *loadHistFromFile(double_t limInfBin, double_t limSupBin, double_t numberBin);
std::string rootFilename(char *inputRootFilePath, char* outputRootPath);
static Double_t fitf(Double_t *x, Double_t *par);
Double_t SPEhistAndPlots(double_t *peak2Valley, double_t *sigma_fit, double_t *occupancy_bynoise);
void sel_pulses(Double_t * rms_noise, Double_t *occupancy_bynoise);
void PulseThresOccupancy();
void getXaxisTime();

// void getTimelimits();
Float_t h_std(vector <float> *v_voltage);
// TTree *T;
int noiseMaxIndex;
 const char *inputPath;
 const char *outputPath;
 const char *param_out_file;
 const char *sel_condition;
// int sampleNumber;
Double_t start_time;
Double_t end_time;
Double_t noMuestras;
Double_t noWaveforms;
Double_t deltaTime;

// void noise();
//  protected:
private:
std::string  inputRootFileName;
std::string outputRootFileName;  

//  const char *path;

// TTree *Tsubset;
float noise_mean;
// TFile *f;

};


























#endif