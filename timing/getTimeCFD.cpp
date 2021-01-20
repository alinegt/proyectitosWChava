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
time.setOutFile(outputFile);
//time.loopOverEntriesTimingRes();
time.loopOverEntries();
std:: string  branch1 = "timech1_le";
std::string   branch2 = "timech1_cfd";
std::string  base_outputFile = time.baseName(outputFile);

time.plotHist( (base_outputFile + "_LE").c_str(), branch1);
time.plotHist( (base_outputFile + "_CFD").c_str(), branch2);
time.closeFile();

}
///*********************************************************************
///======================================
void GetTimeCFD::loopOverEntriesTimingRes()
{
    float a_timech1_cfd{}, a_timeCh2{};
    for (int a_index=0 ; a_index<getNumberOfEntries() ; a_index++)
    {
        m_inputTree->GetEntry(a_index);
        //m_inputTree->GetEntry(0);
        //cout<<"time CFD: "<<getCFDtime(m_ch1)<<endl;
        a_timech1_cfd=getCFDtimeTimingRes(m_ch1, .8, 5,1);
        //  m_resultSignal.clear();
        // m_delaySignal.clear();getInterpolationX
        a_timeCh2=getCFDtimeTimingRes(m_ch2, .8, 5, 2);


        h_timeDiff->Fill(a_timech1_cfd-a_timeCh2);
        m_timeDifference = a_timech1_cfd-a_timeCh2;
        m_timeCh1_cfd=a_timech1_cfd;
        m_timeCh2=a_timeCh2;

        m_outputTree->Fill();
        m_resultSignal_ch1.clear();
        m_delaySignal_ch1.clear();
        m_resultSignal_ch2.clear();
        m_delaySignal_ch2.clear();

    }
    m_outputFile->Write();
    saveHistogram(h_timeDiff);
    std::cout<<"File written"<<std::endl;
   // m_outputFile->Close();


}

///*********************************************************************
///======================================
void GetTimeCFD::loopOverEntries()
{
    float a_timech1_cfd{}, a_timeCh2{};
    
    m_inputTree->Draw("noise>>h_noise", "", "goff");
    TH1F *h_noise = (TH1F *)gDirectory->Get("h_noise");
    float noise_mean = h_noise->GetMean();

    for (int a_index=0 ; a_index<getNumberOfEntries() ; a_index++)
    {
        m_inputTree->GetEntry(a_index);
        auto minimum=std::min_element(  m_ch1->begin(), m_ch1->end() ) ;
        //m_inputTree->GetEntry(0);
        //cout<<"time CFD: "<<getCFDtime(m_ch1)<<endl;
        if (  *minimum  <noise_mean*(-1)  )  {
        m_timeCh1_cfd=getCFDtime(m_ch1, .8, -3);
        m_timeCh1_le =std::min_element(  m_ch1->begin(), m_ch1->end() ) - m_ch1->begin(); ;

        m_outputTree->Fill();
        m_resultSignal_ch1.clear();
        m_delaySignal_ch1.clear();

        }
    }
    m_outputFile->Write();
    // saveHistogram(h_timeDiff);
    std::cout<<"File written"<<std::endl;
//    m_outputFile->Close();


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
float GetTimeCFD::getCFDtimeTimingRes(std::vector<float> *a_signal, float a_fraction, float a_delay, int ch){
    std::vector <float> a_delaySignal;
    std::vector <float> a_resultSignal;    
    bool a_flag = false;
    float a_x1{},a_x2{},a_y1{},a_y2{};
    float a_timeCFD{};
    //cout<<"signal size: "<<a_signal->size();
    ///Negative and delayed signal
    for (long unsigned int a_index=0 ; a_index<a_signal->size();a_index++)
    {
        a_delaySignal.push_back(a_signal->at(a_index)*(-1));

    }
    if (a_delay>0){
    std::rotate(a_delaySignal.begin(), a_delaySignal.begin()+a_delay,a_delaySignal.end());
    }
    else{
    std::rotate(a_delaySignal.begin(), a_delaySignal.begin()+ a_delaySignal.size()+a_delay,a_delaySignal.end());

    }
    //cout<<"a_delay size: "<<a_delaySignal.size();
    ///Sum of the signals
    for (long unsigned int a_index=0 ; a_index<a_signal->size();a_index++)
    {
        a_resultSignal.push_back(a_fraction *a_signal->at(a_index)+a_delaySignal.at(a_index));
    }
    // std::rotate(a_delaySignal.begin(), a_delaySignal.begin()+a_fraction,a_delaySignal.end())

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


        //cout<<a_index<<" "<<m_resultSignal.at(a_index)<<endl;
    }
    if (ch==1) {
        m_resultSignal_ch1= a_resultSignal;
        m_delaySignal_ch1=a_delaySignal;
    } else
    {
        m_resultSignal_ch2=a_resultSignal;
        m_delaySignal_ch2=a_delaySignal;
    }
    
    
    //cout<<"time CFD: "<<a_timeCFD<<endl;
    return a_timeCFD;
}
///======================================
float GetTimeCFD::getCFDtime(std::vector<float> *a_signal, float a_fraction, float a_delay){
    std::vector <float> a_delaySignal;
    std::vector <float> a_resultSignal;    
    //float Xdiv = 0.5; // each sample corresponds to 0.5 ns 
    bool a_flag = false;
    float a_x1{},a_x2{},a_y1{},a_y2{};
    float a_timeCFD{};
    //cout<<"signal size: "<<a_signal->size();
    ///Negative and delayed signal
    for (long unsigned int a_index=0 ; a_index<a_signal->size();a_index++)
    {
        a_delaySignal.push_back(a_signal->at(a_index)*(-1));

    }
    if (a_delay>0){
    std::rotate(a_delaySignal.begin(), a_delaySignal.begin()+a_delay,a_delaySignal.end());
    }
    else{
    std::rotate(a_delaySignal.begin(), a_delaySignal.begin()+ a_delaySignal.size()+a_delay,a_delaySignal.end());

    }
    //cout<<"a_delay size: "<<a_delaySignal.size();
    ///Sum of the signals
    for (long unsigned int a_index=0 ; a_index<a_signal->size();a_index++)
    {
        a_resultSignal.push_back(a_fraction *a_signal->at(a_index)+a_delaySignal.at(a_index));
    }
    // std::rotate(a_delaySignal.begin(), a_delaySignal.begin()+a_fraction,a_delaySignal.end())

    for(long unsigned int a_index=0 ; a_index<a_resultSignal.size();a_index++)
    {
        if ((a_resultSignal.at(a_index)<-0.02)&&(a_flag==false))
        {a_flag = true;}
        if ((a_resultSignal.at(a_index)>0.00)&&(a_flag==true)){
            //cout<<a_index<<" "<<a_resultSignal.at(a_index)<<endl;
                a_x1 = (float)a_index-1;
                a_x2 = (float)a_index;
                a_y1 = a_resultSignal.at(a_index-1);
                a_y2 = a_resultSignal.at(a_index);
                a_flag = false;

                a_timeCFD = getInterpolationX(a_x1,a_x2,a_y1,a_y2);
                a_timeCFD = a_timeCFD;
                //cout<<a_x1<<" "<<a_x2<<" "<<a_y1<<" "<<a_y2<<endl;
        }


        //cout<<a_index<<" "<<m_resultSignal.at(a_index)<<endl;
    }
    // if (ch==1) {
        m_resultSignal_ch1= a_resultSignal;
        m_delaySignal_ch1=a_delaySignal;
    // } else
    // {
    //     m_resultSignal_ch2=a_resultSignal;
    //     m_delaySignal_ch2=a_delaySignal;
    // }
    
    
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
    m_inputTree -> SetBranchAddress("voltage",&m_ch1);
   // m_inputTree -> SetBranchAddress("ch2",&m_ch2);
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


/**
 * @brief Get the Time CFD::set Out File object
 * 
 * @param a_outputFile 
 */

/// Save Tree root in a output file
void GetTimeCFD::setOutFile(const std::string & a_outputFile){

    string outFile = a_outputFile;
    m_outputFile = new TFile(outFile.c_str(), "RECREATE");
    m_outputTree = new TTree("Tree", "Difference time info");
    m_outputTree -> Branch("timeDifference",&m_timeDifference);
    m_outputTree -> Branch("resultSignal_ch1",&m_resultSignal_ch1);
    m_outputTree -> Branch("resultSignal_ch2",&m_resultSignal_ch2);
    m_outputTree -> Branch("delaySignal_ch1",&m_delaySignal_ch1);
    m_outputTree -> Branch("delaySignal_ch2",&m_delaySignal_ch2);
    m_outputTree -> Branch("ch1",&m_ch1);
    m_outputTree -> Branch("ch2",&m_ch2);
    m_outputTree -> Branch("timech1_cfd",&m_timeCh1_cfd);
    m_outputTree -> Branch("timech2",&m_timeCh2);
    m_outputTree -> Branch("timech1_le",&m_timeCh1_le);  // Leading edge analysis



    return;
}

void GetTimeCFD::plotHist(const std::string & a_outputFile, const std::string & branch){
TCanvas *c = new TCanvas("c", "A3", 1000, 700);
// m_outputTree->Print("timech1_cfd");
int xmin= 50;
int xmax= 75;
  std::string titleHist= a_outputFile;
 char *titleHist_arr;
  titleHist_arr = &titleHist[0];
TH1F *hframe = new TH1F("hframe","CFD Results",50,xmin,xmax);
  hframe->SetTitle(Form("%s; Time [ns] ; Counts", titleHist_arr));

hframe->Draw();
 hframe->GetYaxis()->SetRangeUser(0., 400.);
// hframe->GetXaxis()->SetRangeUser(0., 110.);  

 m_outputTree->Draw(Form( " %s * %s>> h_%s",branch.c_str(), Xdiv.c_str(), a_outputFile.c_str())   ,"","same"); 
 TH1F *h_temp = (TH1F*)gPad->GetPrimitive(Form("h_%s", a_outputFile.c_str() ));
 h_temp->SetName(Form("h_%s",  a_outputFile.c_str()) ); 
 h_temp->SetTitle(Form("%s",  a_outputFile.c_str()) );
  c->Update();
  c->SetGrid();
  c->cd();
   std::string outPath = "./data/plots/";
   c->Print((outPath + a_outputFile + ".png").c_str());
   
   h_temp->Write();
   c->Close();

return;

}

void GetTimeCFD::closeFile()
{
m_outputFile->Close();
return;
}

std::string GetTimeCFD::baseName(std::string const &path)
{
  std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
  std::string::size_type const p(base_filename.find_last_of('.'));
  return base_filename.substr(0, p);
}