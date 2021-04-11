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
//time.nofSamples = std::stoi(argv[3]);
time.start_time=0;
time.end_time=0;
time.loadDataFile(inputFile);
time.getXaxisTime(inputFile);

cout<<"The tree has: "<<time.getNumberOfEntries()<<" events"<<endl;
time.setOutFile(outputFile);
//time.loopOverEntriesTimingRes();
time.loopOverEntries();
std:: string  branch1 = "timech1_50";
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
    
    m_inputTree->Draw("noise>>h_noise", "", "goff");
    TH1F *h_noise = (TH1F *)gDirectory->Get("h_noise");
    noise_mean = h_noise->GetMean();
    cout<<"noise_mean= "<< noise_mean<<endl;


    for (int a_index0=0 ; a_index0<getNumberOfEntries() ; a_index0++)
    {
        m_inputTree->GetEntry(a_index0);
        // float noiseFromPlot=0.03;
        auto minimum=std::min_element(  m_ch1->begin(), m_ch1->end() ) ;
        //m_inputTree->GetEntry(0);
        //cout<<"time CFD: "<<getCFDtime(m_ch1)<<endl;
       if ( m_pulses_b==1 )  {
       // if (  *minimum  < noise_mean*(-4)  )  {
        m_timeCh1_cfd=getCFDtime(m_ch1,m_time, .75, -3 );
        GetTimeLeadEdge(m_ch1, m_time); 
        
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

void GetTimeCFD::GetTimeLeadEdge(std::vector<float> *a_vector, std::vector<float> *a_time){
    auto peak= *min_element( a_vector->begin(), a_vector->end() );
    bool flag10= false;
    bool flag50= false;
    bool flag90= false;

   for(long unsigned int a_index1=1 ; a_index1<a_vector->size()-1;a_index1++)
    {
    //   cout<<a_index1<<endl;
    //   cout<<0.1*peak<<endl;

    //    cout<<a_vector->size()<<endl;

        if (  (a_vector->at(a_index1) < (float)0.1*peak )  && (flag10==false) ) 
        {
            // cout<<"time10"<<endl;

            m_timeCh1_10 = getTime(a_index1, a_vector, a_time);
            
            flag10= true;
        }

         if ((a_vector->at(a_index1) < 0.5*(double)peak )  && (flag50==false) ) 
        {
            m_timeCh1_50 = getTime(a_index1, a_vector,a_time);
            flag50= true;
                        // cout<<"time50"<<endl;

        }
          if ( (a_vector->at(a_index1) < 0.9*(double)peak  ) && (flag90==false) )
        {
            m_timeCh1_90 = getTime(a_index1, a_vector,a_time);
            flag90=true;
                        // cout<<"time90"<<endl;

        }
    }
}
float GetTimeCFD::getTime(long unsigned int a_index2, std::vector<float> *a_vector2, std::vector<float> *a_time){

                   
            float cfdTime;
            float a_x1,a_x2,a_y1,a_y2=0;

               a_x1 = a_time-> at (a_index2-1);
                a_x2 = a_time-> at (a_index2);

                a_y1 = a_vector2->at(a_index2-1);

                a_y2 = a_vector2->at(a_index2);

                cfdTime = getInterpolationX(a_x1,a_x2,a_y1,a_y2);
                
                return cfdTime;
}

///======================================
/// Zero crossing (y=0)
float GetTimeCFD::getInterpolationX(float a_x1, float a_x2, float a_y1, float a_y2)
{
    
    float a_interpolationXNum=0;
    float a_slope=0;
    a_slope = (a_y2-a_y1)/(a_x2-a_x1);
    a_interpolationXNum = a_x1-(a_y1/a_slope);
    // cout<<a_interpolationXNum/1e-9<<endl;
    return a_interpolationXNum/1e-9;
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

    for(long unsigned int a_index=1 ; a_index<a_resultSignal.size()-1;a_index++)
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
float GetTimeCFD::getCFDtime(std::vector<float> *a_signal, std::vector<float> *a_time, float a_fraction, float a_delay){
    std::vector <float> a_delaySignal;
    std::vector <float> a_resultSignal;    
    bool a_flag = false;
    float a_timeCFD=0;
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
        m_resultSignal_ch1= a_resultSignal;
        m_delaySignal_ch1= a_delaySignal;

    for(long unsigned int a_index=0 ; a_index<a_resultSignal.size();a_index++)
    {
        if ((a_resultSignal.at(a_index)< noise_mean*(-10))&&(a_flag==false))
        {a_flag = true;}
        if ((a_resultSignal.at(a_index)>0.00)&&(a_flag==true)){
            //cout<<a_index<<" "<<a_resultSignal.at(a_index)<<endl;
            
            
                //             a_x1 = (float)a_index-1;
                // a_x2 = (float)a_index;
                // a_y1 = a_resultSignal.at(a_index-1);
                // a_y2 = a_resultSignal.at(a_index);

                a_timeCFD = getTime(a_index,&m_resultSignal_ch1, a_time);
                a_flag = false;

                //cout<<a_x1<<" "<<a_x2<<" "<<a_y1<<" "<<a_y2<<endl;
        }


        //cout<<a_index<<" "<<m_resultSignal.at(a_index)<<endl;
    }
    // if (ch==1) {
    // } else
    // {
    //     m_resultSignal_ch2=a_resultSignal;
    //     m_delaySignal_ch2=a_delaySignal;
    // }
    
    
    //cout<<"time CFD: "<<a_timeCFD<<endl;
    return a_timeCFD+a_delay;
}





///======================================
bool GetTimeCFD::loadDataFile(const std::string & a_inputFile)
{
    string inFile = a_inputFile;
    auto f = new TFile(inFile.c_str(),"READONLY");
    m_inputTree = (TTree*)f->Get("T");
    TVectorD *dataParams = (TVectorD *)m_inputTree->GetUserInfo()->At(0);
    nofSamples= dataParams[0][0]-1;
    std::cout<< dataParams[0][0]-1<<std::endl;
    nofWaveforms= dataParams[0][1];
    deltaTime=dataParams[0][2];

    m_inputTree -> SetBranchAddress("voltage",&m_ch1);
    m_inputTree -> SetBranchAddress("time", &m_time );
    m_inputTree -> SetBranchAddress("pulses", &m_pulses_b );
    m_inputTree -> SetBranchAddress("noOutliers", &m_noOutlier_b );

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
    m_outputTree -> Branch("timech1_10",&m_timeCh1_10);
    m_outputTree -> Branch("timech1_50",&m_timeCh1_50);
    m_outputTree -> Branch("timech1_90",&m_timeCh1_90);
    m_outputTree -> Branch("time", &m_time);
    return;
}

void GetTimeCFD::plotHist(const std::string & a_outputFile, const std::string & branch){
TCanvas *c = new TCanvas("c", "A3", 1000, 700);
// m_outputTree->Print("timech1_cfd");
// int xmin= 40;
// int xmax= 70;
  std::string titleHist= a_outputFile;
 char *titleHist_arr;
  titleHist_arr = &titleHist[0];
// TH1F *hframe = new TH1F("hframe","CFD Results",50,xmin,xmax);

// hframe->Draw();
//  hframe->GetYaxis()->SetRangeUser(0., 400.);
// // hframe->GetXaxis()->SetRangeUser(0., 110.);  

 m_outputTree->Draw(Form( " %s >> h_%s(640,%f,%f)",branch.c_str(), a_outputFile.c_str(), start_time, end_time) ,""); 
 TH1F *h_temp = (TH1F*)gPad->GetPrimitive(Form("h_%s", a_outputFile.c_str() ));
   h_temp->SetTitle(Form("%s; Time [ns] ; Counts", titleHist_arr));
  h_temp->SetLineWidth(2);

 //h_temp->GetXaxis()->SetNdivisions(200);
//  h_temp->SetMinimum(0.1);
//  h_temp->SetMaximum(10000);
 h_temp->GetYaxis()->SetRangeUser(0.1,1e3);

 h_temp->SetName(Form("h_%s",  a_outputFile.c_str()) ); 
 h_temp->SetTitle(Form("%s",  a_outputFile.c_str()) );


// h_temp->Fit("gaus","0","",start_time+45,start_time+75);
h_temp->Fit("gaus","","",start_time+225,start_time+238);
// h_temp->GetFunction("gaus")->SetLineColor(kBlack);

//    h_temp->GetFunction("gaus")->SetLineColor(kBlack);
gPad->Update();

/*
TF1 *parGaus = (TF1 *)h_temp->GetListOfFunctions()->FindObject("gaus");

   TF1 *f1 = new TF1("f1","gaus",start_time+80,start_time+100);
// // set initial parameters (not really needed for gaus)
f1->SetParameters(parGaus->GetParameter(0), parGaus->GetParameter(1), parGaus->GetParameter(2) ); 
 f1->Draw("sames");
    f1->SetLineColor(kBlack);

gPad->Update();


  c->Update();
*/
TPaveStats *ps2 = (TPaveStats *)h_temp->GetListOfFunctions()->FindObject("stats");
  ps2->SetOptFit(1110); 
   ps2->SetTextColor(kBlack);
    ps2->SetX1NDC(0.6);
      ps2->SetX2NDC(0.85);
      ps2->SetY1NDC(0.6);
      ps2->SetY2NDC(0.95);
  c->Update();
/*
TH1F *h_after = (TH1F *)h_temp->Clone("h_after");

h_after->Fit("expo","","sames",start_time+100,start_time+115);
   h_after->GetFunction("expo")->SetLineColor(kRed);


TPaveStats *ps3 = (TPaveStats *)h_after->GetListOfFunctions()->FindObject("stats");
  ps3->SetOptFit(1110); 
   ps3->SetTextColor(kRed);
    ps3->SetX1NDC(0.2);
      ps3->SetX2NDC(0.4);
      ps3->SetY1NDC(0.6);
      ps3->SetY2NDC(0.95);
  c->Update();

// TH1F *h_landau = (TH1F *)h_temp->Clone("h_landau");

// h_landau->Fit("landau","","sames",start_time+35,start_time+110);
//    h_landau->GetFunction("landau")->SetLineColor(kPink);


// TPaveStats *ps4 = (TPaveStats *)h_landau->GetListOfFunctions()->FindObject("stats");
//   ps4->SetOptFit(1110); 
//    ps4->SetTextColor(kPink);
//     ps4->SetX1NDC(0.6);
//       ps4->SetX2NDC(0.85);
//       ps4->SetY1NDC(0.3);
//       ps4->SetY2NDC(0.6);


  c->Update();


gPad->Update();

  //h_temp->SetStats(0);

//   gStyle->SetOptFit(1110); 
 */
  gPad->SetLogy();
 
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


/**
 * @brief Get the limits in the x axis from time values
 * 
 */
void GetTimeCFD::getXaxisTime(const std::string & a_inputFile){
    // TFile *f0 = TFile::Open("noCoil2.root", "read");
    // TFile *f0 = TFile::Open(inputPath, "read");
    string inFile = a_inputFile;
    auto f = new TFile(inFile.c_str(),"READONLY");
    TTreeReader aReader("T",f);
    TTreeReaderValue<std::vector<float>> timeRV(aReader, "time");
    unsigned int it = 0;
    while (aReader.Next())
    {
        for (auto &&value : *timeRV)
        {
            if (it == 0)
            {
                start_time= round(value*1e9);
                cout << value << endl;
            }

            if (it ==(nofSamples))
            {
              end_time= round(value*1e9);
                cout << value << endl;
                break;
            }
            it++;
        }
        it = 0;

        break;
    }
    // delete f0;
}
