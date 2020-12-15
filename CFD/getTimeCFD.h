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

using namespace std;

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

private:
    TTree* m_inputTree;
    TTree* m_outputTree;
    TFile* m_outputFile;
    std::vector<float> *m_ch1=0;
    std::vector<float> *m_ch2=0;
    std::vector<float> m_delaySignal_ch1;
    std::vector<float> m_resultSignal_ch1;
    std::vector<float> m_delaySignal_ch2;
    std::vector<float> m_resultSignal_ch2;
    float m_timeCh1;
    float m_timeCh2;

    bool m_fileLoaded;
    float m_timeDifference;
    //parameters of the histogram must be changed in order to set a proper range
    TH1F *h_timeDiff = new TH1F("h_timeDiff", "h_timeDiff", 70, 20, 25);

};

#endif

