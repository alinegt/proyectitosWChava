#ifndef _GET_TIME_CFD_H
#define _GET_TIME_CFD_H

//c++ includes

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <algorithm> // std::min_element
//#include <iterator>  // std::begin, std::end

//ROOT
#include <TROOT.h>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TTreeReader.h"
using namespace std;
// const float Xdiv = 0.5;
#define NAME_OF( v ) #v
class GetTimeCFD
{

public:
    GetTimeCFD();
    ~GetTimeCFD();
    bool loadDataFile(const std::string & a_inputFile);
    Long64_t getNumberOfEntries();
    void loopOverEntriesTimingRes();
    void loopOverEntries();
    float getCFDtimeTimingRes(std::vector<float> *a_signal, float a_fraction, float a_delay, int ch);
    float getCFDtime(std::vector<float> *a_signal, float a_fraction, float a_delay);
    float getInterpolationX(float a_x1, float a_x2, float a_y1, float a_y2);
    void saveHistogram(TH1F *a_histogram);
    void setOutFile(const std::string & a_outputFile);
    void plotHist(const std::string & a_outputFile, const std::string & branch);
    void GetTimeLeadEdge(std::vector<float> *a_vector);
    float getTime(long unsigned int a_index2, std::vector<float> *a_vector2);
    void getXaxisTime(const std::string & a_inputFile);

    // void plotHistLE(const std::string & a_outputFile);
    void closeFile();

    std::string baseName(std::string const &path);
   Double_t start_time=0;
    Double_t end_time=0;
    int numberOfSamples=0;

private:
    TTree* m_inputTree;
    TTree* m_outputTree;
    TFile* m_outputFile;
    std::vector<float> *m_time=0; // Time vector from scope
    std::vector<float> *m_ch1=0;
    std::vector<float> *m_ch2=0;
    std::vector<float> m_delaySignal_ch1;
    std::vector<float> m_resultSignal_ch1;
    std::vector<float> m_delaySignal_ch2;
    std::vector<float> m_resultSignal_ch2;
    bool m_pulses_b;
    bool m_noOutlier_b;
    float m_timeCh1_cfd; 
    float m_timeCh2;
    int m_timeCh1_le;
    float m_timeCh1_10;
    float m_timeCh1_50;
    float m_timeCh1_90;
    float m_timeCh2_le;
    std::string Xdiv= "0.4688";
    bool m_fileLoaded;
    float m_timeDifference;
 
    //parameters of the histogram must be changed in order to set a proper range
    TH1F *h_timeDiff = new TH1F("h_timeDiff", "h_timeDiff", 70, 20, 25);

};
#endif

