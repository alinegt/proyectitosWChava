#include "getTimeCFD.h"

/// Constructor
GetTimeCFD::GetTimeCFD(): m_fileLoaded(0) {}
/// Destructor
GetTimeCFD::~GetTimeCFD() {}
///*********************************************************************
/// Main
///*********************************************************************
int main(int argc, char **argv) {
std::string inputFile = argv[1];

GetTimeCFD time;
time.loadDataFile(inputFile);
cout<<"The tree has: "<<time.getNumberOfEntries()<<" events"<<endl;
time.forLoop();
}
///*********************************************************************
///======================================
void GetTimeCFD::forLoop()
{
    //for (int a_index=0 ; a_index<getNumberOfEntries() ; a_index++) {
        //m_inputTree->GetEntry(a_index);
    m_inputTree->GetEntry(0);

        getCFDtime(m_ch1);
    //s}
}


///======================================
float GetTimeCFD::getCFDtime(std::vector<float> *a_signal){
    cout<<"here"<<endl;
    std::vector <float> a_delaySignal;
    std::vector <float> a_resultSignal;
    int a_delayIndex = 25; //25 samples of delay
    float a_fraction = 0.7;
    cout<<"signal size: "<<a_signal->size();
    ///Negative and delayed signal
    for (long unsigned int a_index=0 ; a_index<a_signal->size()-a_delayIndex;a_index++)
    {
        a_delaySignal.push_back(-a_signal->at(a_index+a_delayIndex));

    }

    cout<<"a_delay size: "<<a_delaySignal.size();
    ///Sum of the signals
    for (long unsigned int a_index=0 ; a_index<a_signal->size()-a_delayIndex;a_index++)
    {
        cout<<a_index<<" "<<a_signal->at(a_index)<<" "<<-a_signal->at(a_index+a_delayIndex)<<endl;
        a_resultSignal.push_back(a_fraction *a_signal->at(a_index)+a_delaySignal.at(a_index));
    }
    cout<<"Result signal"<<endl;

    for(long unsigned int a_index=0 ; a_index<a_resultSignal.size();a_index++)
    {

        cout<<a_index<<" "<<a_resultSignal.at(a_index)<<endl;
    }
    return 0;
}
///======================================

bool GetTimeCFD::loadDataFile(const std::string & a_inputFile)
{
    string inFile = a_inputFile;
    auto f = new TFile(inFile.c_str(),"READONLY");
    m_inputTree = (TTree*)f->Get("T");
    m_inputTree -> SetBranchAddress("ch1",&m_ch1);
    m_inputTree -> SetBranchAddress("ch2",&m_ch2);
    m_fileLoaded = true;
    return m_fileLoaded;
}
///======================================
Long64_t GetTimeCFD::getNumberOfEntries()
{
    int a_entries{};
    if (m_fileLoaded==true)
        a_entries=m_inputTree->GetEntries();
    return a_entries;
}
///======================================

