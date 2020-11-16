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
std::string outputFile = argv[2];

GetTimeCFD time;
time.loadDataFile(inputFile);
cout<<"The tree has: "<<time.getNumberOfEntries()<<" events"<<endl;

time.saveFile(outputFile);
time.forLoop();
}
///*********************************************************************
///======================================
void GetTimeCFD::forLoop()
{
    for (int a_index=0 ; a_index<getNumberOfEntries() ; a_index++)
    {
        m_inputTree->GetEntry(a_index);
        //m_inputTree->GetEntry(0);
        //cout<<"time CFD: "<<getCFDtime(m_ch1)<<endl;
        h_timeDiff->Fill(getCFDtime(m_ch1)-getCFDtime(m_ch2));
        m_timeDifference = getCFDtime(m_ch1) - getCFDtime(m_ch2);

        m_outputTree->Fill();
    }
    m_outputFile->Write();
    saveHistogram(h_timeDiff);
    std::cout<<"File written"<<std::endl;
    m_outputFile->Close();

}


///======================================

void GetTimeCFD::saveHistogram(TH1F *a_histogram){

    TCanvas *c = new TCanvas ("c","A3",700,700);
    const char* a_textFileName = a_histogram->GetName();
    std::string a_nameFileStr = a_textFileName;
    a_nameFileStr=a_nameFileStr+".png";
    const char* a_nameFile = a_nameFileStr.c_str();;

    c->SetGrid();

    //gPad->SetLogz();
    //gPad->SetLogy();
    //gPad->SetLogx();
    //gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);
    //gStyle->SetStatX(0.9);
    //gStyle->SetStatY(0.9);
    a_histogram -> GetYaxis() -> SetTitleOffset(1.2);
    a_histogram -> GetXaxis() -> SetTitleOffset(1.2);
    a_histogram->Draw();
    //TPaveText *labeD = new TPaveText(0.5,0.1,0.9,0.2,"brNDC");
    //labeD->AddText(correlation.c_str());
    //labeD->Draw();
    c->Print(a_nameFile);
    c->cd();

}
///======================================
float GetTimeCFD::getCFDtime(std::vector<float> *a_signal){
    bool a_flag = false;
    float a_x1{},a_x2{},a_y1{},a_y2{};
    std::vector <float> a_delaySignal;
    std::vector <float> a_resultSignal;
    int a_delayIndex = 5; //25 samples of delay
    float a_fraction = 0.7;
    float a_timeCFD{};
    //cout<<"signal size: "<<a_signal->size();
    ///Negative and delayed signal
    for (long unsigned int a_index=0 ; a_index<a_signal->size()-a_delayIndex;a_index++)
    {
        a_delaySignal.push_back(-a_signal->at(a_index+a_delayIndex));

    }

    //cout<<"a_delay size: "<<a_delaySignal.size();
    ///Sum of the signals
    for (long unsigned int a_index=0 ; a_index<a_signal->size()-a_delayIndex;a_index++)
    {
        //cout<<a_index<<" "<<a_signal->at(a_index)<<" "<<-a_signal->at(a_index+a_delayIndex)<<endl;
        a_resultSignal.push_back(a_fraction *a_signal->at(a_index)+a_delaySignal.at(a_index));
    }



    for(long unsigned int a_index=0 ; a_index<a_resultSignal.size();a_index++)
    {
        if ((a_resultSignal.at(a_index)<-100.)&&(a_flag==false))
        {a_flag = true;}
        if ((a_resultSignal.at(a_index)>0)&&(a_flag==true)){
            //cout<<a_index<<" "<<a_resultSignal.at(a_index)<<endl;
                a_x1 = (float)a_index-1;
                a_x2 = (float)a_index;
                a_y1 = a_resultSignal.at(a_index-1);
                a_y2 = a_resultSignal.at(a_index);
                a_flag = false;

                a_timeCFD = getInterpolationX(a_x1,a_x2,a_y1,a_y2);
                //cout<<a_x1<<" "<<a_x2<<" "<<a_y1<<" "<<a_y2<<endl;
        }


        //cout<<a_index<<" "<<a_resultSignal.at(a_index)<<endl;
    }
    //cout<<"time CFD: "<<a_timeCFD<<endl;
    return a_timeCFD;
}
///======================================
/// Zero crossing (y=0)
float GetTimeCFD::getInterpolationX(float a_x1, float a_x2, float a_y1, float a_y2)
{
    float a_interpolationXNum{};
    float a_slope{};
    a_slope = (a_y2-a_y1)/(a_x2-a_x1);
    a_interpolationXNum = a_x1-(a_y1/a_slope);

    return a_interpolationXNum;
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
/// Save Tree root in a output file
void GetTimeCFD::saveFile(const std::string & a_outputFile){

    string outFile = a_outputFile;
    m_outputFile = new TFile(outFile.c_str(), "RECREATE");
    m_outputTree = new TTree("Tree", "Difference time info");
    m_outputTree -> Branch("timeDifference",&m_timeDifference);

    return;
}
