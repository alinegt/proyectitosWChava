//This program reads the output ntuple root file from the muonAmplitudeAnalysis.cpp.
//It is recomended that execute this program un a new folder, as it will save images of every fit done,
//and data files as well. Run with the make file to generate an executable.

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "TMath.h"
#include <TROOT.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TLegend.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "Riostream.h"
#include "TClass.h"
#include "TView.h"
#include "TPaveStats.h"
#include "TGaxis.h"

using namespace std; 

// Double to string
template <typename T>
std::string to_string(T const& value) {
    stringstream sstr;
    sstr << value;
    return sstr.str();
}

// Converts an int to a string
string int_to_string(int integer) {
  ostringstream stst;
  stst << integer;
  return stst.str();
}
void generateTwoHistogramCanvas(TH1F *a_histogram1,TH1F *a_histogram2){
    TCanvas *c = new TCanvas ("c","A3",1000,700);
    //TPad *pad1 = new TPad("pad1","",0,0,1,1);
    //TPad *pad2 = new TPad("pad2","",0,0,1,1);
    //pad2->SetFillStyle(4000);
    //pad1->Draw();
    //pad1->cd();
    const char* a_textFileName = a_histogram1->GetName();
    std::string a_nameFileStr = a_textFileName;
    a_nameFileStr="temp/"+a_nameFileStr+".png";
    const char* a_nameFile = a_nameFileStr.c_str();

    a_histogram1->Draw();
    c->Update();
    TPaveStats *ps1 = (TPaveStats*)a_histogram1->GetListOfFunctions()->FindObject("stats");
    ps1->SetX1NDC(0.15); ps1->SetX2NDC(0.35);
    ps1->SetTextColor(kBlue);
    //pad1->Modified();
    a_histogram2->SetLineColor(kRed);
    a_histogram2->Draw("sames");
    c->Update();
    TPaveStats *ps2 = (TPaveStats*)a_histogram2->GetListOfFunctions()->FindObject("stats");
    ps2->SetX1NDC(0.65); ps2->SetX2NDC(0.85);
    ps2->SetTextColor(kRed);

    TLegend *leg =  new TLegend(0.65,0.35,0.9,0.45);
    //leg->SetHeader("");
    leg->AddEntry(a_histogram1,"When #DeltaT^{th} uses random numbers ");
    leg->AddEntry(a_histogram2,"When #DeltaT^{th} uses position resolutions");
    leg->Draw();

    c->Update();
    c->Print(a_nameFile);
    c->cd();

}

void generateCanvas1D(TH1F *a_histogram){

    TCanvas *c = new TCanvas ("c","A3",700,700);
    const char* a_textFileName = a_histogram->GetName();
    std::string a_nameFileStr = a_textFileName;
    a_nameFileStr="temp/"+a_nameFileStr+".png";
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

int main(int argc, char **argv){
cout << argv[1]<< endl;

 string fileName = argv[1];

 gROOT->SetBatch(); //Avoids showing plots during the execution

 //-----------------------------------------------------------------------------

   TObjArray Hlist(0);
   TApplication app("app",0,0);



///---------------------------------------------------------------------------
///¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡INPUT FILE!!!!!!!!!!!!!!!!!!!
TFile *f = new TFile(fileName.c_str(),"READONLY");
TTree *events = (TTree*)f->Get("Tree");
ULong64_t nentries = (Int_t)events->GetEntries();


///---------------------------------------------------------------------------
///  Setting Branches and variables
///---------------------------------------------------------------------------
Double_t deltaTrue16;
events->SetBranchAddress("deltaTrue16", &deltaTrue16);
Double_t deltaTrue24;
events->SetBranchAddress("deltaTrue24", &deltaTrue24);
Double_t deltaTrue32;
events->SetBranchAddress("deltaTrue32", &deltaTrue32);
Double_t deltaTrue40;
events->SetBranchAddress("deltaTrue40", &deltaTrue40);
Double_t deltaTrue48;
events->SetBranchAddress("deltaTrue48", &deltaTrue48);
Double_t deltaTrue56;
events->SetBranchAddress("deltaTrue56", &deltaTrue56);
Double_t deltaTrue64;
events->SetBranchAddress("deltaTrue64", &deltaTrue64);
Double_t deltaMeasured16;
events->SetBranchAddress("deltaMeasured16", &deltaMeasured16);
Double_t deltaMeasured24;
events->SetBranchAddress("deltaMeasured24", &deltaMeasured24);
Double_t deltaMeasured32;
events->SetBranchAddress("deltaMeasured32", &deltaMeasured32);
Double_t deltaMeasured40;
events->SetBranchAddress("deltaMeasured40", &deltaMeasured40);
Double_t deltaMeasured48;
events->SetBranchAddress("deltaMeasured48", &deltaMeasured48);
Double_t deltaMeasured56;
events->SetBranchAddress("deltaMeasured56", &deltaMeasured56);
Double_t deltaMeasured64;
events->SetBranchAddress("deltaMeasured64", &deltaMeasured64);

Double_t deltaTtrueTimeResolution16;
events->SetBranchAddress("deltaTtrueTimeResolution16", &deltaTtrueTimeResolution16);
Double_t deltaTtrueTimeResolution24;
events->SetBranchAddress("deltaTtrueTimeResolution24", &deltaTtrueTimeResolution24);
Double_t deltaTtrueTimeResolution32;
events->SetBranchAddress("deltaTtrueTimeResolution32", &deltaTtrueTimeResolution32);
Double_t deltaTtrueTimeResolution40;
events->SetBranchAddress("deltaTtrueTimeResolution40", &deltaTtrueTimeResolution40);
Double_t deltaTtrueTimeResolution48;
events->SetBranchAddress("deltaTtrueTimeResolution48", &deltaTtrueTimeResolution48);
Double_t deltaTtrueTimeResolution56;
events->SetBranchAddress("deltaTtrueTimeResolution56", &deltaTtrueTimeResolution56);
Double_t deltaTtrueTimeResolution64;
events->SetBranchAddress("deltaTtrueTimeResolution64", &deltaTtrueTimeResolution64);

Double_t experimentalTimeMeasured16;
events->SetBranchAddress("experimentalTimeMeasured16",&experimentalTimeMeasured16);
Double_t experimentalTimeMeasured24;
events->SetBranchAddress("experimentalTimeMeasured24", &experimentalTimeMeasured24);
Double_t experimentalTimeMeasured32;
events->SetBranchAddress("experimentalTimeMeasured32", &experimentalTimeMeasured32);
Double_t experimentalTimeMeasured40;
events->SetBranchAddress("experimentalTimeMeasured40", &experimentalTimeMeasured40);
Double_t experimentalTimeMeasured48;
events->SetBranchAddress("experimentalTimeMeasured48", &experimentalTimeMeasured48);
Double_t experimentalTimeMeasured56;
events->SetBranchAddress("experimentalTimeMeasured56", &experimentalTimeMeasured56);
Double_t experimentalTimeMeasured64;
events->SetBranchAddress("experimentalTimeMeasured64", &experimentalTimeMeasured64);


//---------------------------------------------------------------------------
//  Files
//---------------------------------------------------------------------------

///---------------------------------------------------------------------------
///  Histograms 1D and 2D
///---------------------------------------------------------------------------



///~~~~~~~~~~~~~~~
TH1F *expectedTrueTimeRes16 = new TH1F("expectedTrueTimeRes16","Bar 16; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes16->GetYaxis()->SetTitle("counts");
TH1F *expectedTrueTimeRes24 = new TH1F("expectedTrueTimeRes24","Bar 24; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes24->GetYaxis()->SetTitle("counts");
TH1F *expectedTrueTimeRes32 = new TH1F("expectedTrueTimeRes32","Bar 32; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes32->GetYaxis()->SetTitle("counts");
TH1F *expectedTrueTimeRes40 = new TH1F("expectedTrueTimeRes40","Bar 40; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes40->GetYaxis()->SetTitle("counts");
TH1F *expectedTrueTimeRes48 = new TH1F("expectedTrueTimeRes48","Bar 48; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes48->GetYaxis()->SetTitle("counts");
TH1F *expectedTrueTimeRes56 = new TH1F("expectedTrueTimeRes56","Bar 56; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes56->GetYaxis()->SetTitle("counts");
TH1F *expectedTrueTimeRes64 = new TH1F("expectedTrueTimeRes64","Bar 64; #Delta T^{th}_{true Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
expectedTrueTimeRes64->SetMarkerStyle(7);
expectedTrueTimeRes64->SetMarkerSize(7);
expectedTrueTimeRes64->GetYaxis()->SetTitle("counts");


///~~~~~~~~~~~~~~
TH1F *measuredVsTrueTimeRes16 = new TH1F("measuredVsTrueTimeRes16","Bar 16; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes16->GetYaxis()->SetTitle("counts");
TH1F *measuredVsTrueTimeRes24 = new TH1F("measuredVsTrueTimeRes24","Bar 24; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes24->GetYaxis()->SetTitle("counts");
TH1F *measuredVsTrueTimeRes32 = new TH1F("measuredVsTrueTimeRes32","Bar 32; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes32->GetYaxis()->SetTitle("counts");
TH1F *measuredVsTrueTimeRes40 = new TH1F("measuredVsTrueTimeRes40","Bar 40; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes40->GetYaxis()->SetTitle("counts");
TH1F *measuredVsTrueTimeRes48 = new TH1F("measuredVsTrueTimeRes48","Bar 48; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes48->GetYaxis()->SetTitle("counts");
TH1F *measuredVsTrueTimeRes56 = new TH1F("measuredVsTrueTimeRes56","Bar 56; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes56->GetYaxis()->SetTitle("counts");
TH1F *measuredVsTrueTimeRes64 = new TH1F("measuredVsTrueTimeRes64","Bar 64; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true} (ns)",100,-0.2,1);
measuredVsTrueTimeRes64->SetMarkerStyle(7);
measuredVsTrueTimeRes64->SetMarkerSize(7);
measuredVsTrueTimeRes64->GetYaxis()->SetTitle("counts");

///~~~~~~~~~~~~~~
TH1F *differenceTimeHis16 = new TH1F("differenceTimeHis16","Bar 16; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis16->GetYaxis()->SetTitle("counts");
TH1F *differenceTimeHis24 = new TH1F("differenceTimeHis24","Bar 24; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis24->GetYaxis()->SetTitle("counts");
TH1F *differenceTimeHis32 = new TH1F("differenceTimeHis32","Bar 32; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis32->GetYaxis()->SetTitle("counts");
TH1F *differenceTimeHis40 = new TH1F("differenceTimeHis40","Bar 40; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis40->GetYaxis()->SetTitle("counts");
TH1F *differenceTimeHis48 = new TH1F("differenceTimeHis48","Bar 48; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis48->GetYaxis()->SetTitle("counts");
TH1F *differenceTimeHis56 = new TH1F("differenceTimeHis56","Bar 56; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis56->GetYaxis()->SetTitle("counts");
TH1F *differenceTimeHis64 = new TH1F("differenceTimeHis64","Bar 64; #Delta T^{th}_{measured Time Res} - #Delta T^{th}_{true Time Res}  (ns)",100,-0.6,1);
differenceTimeHis64->SetMarkerStyle(7);
differenceTimeHis64->SetMarkerSize(7);
differenceTimeHis64->GetYaxis()->SetTitle("counts");

///~~~~~~~~~~~~~~
TH1F *deltaT116 = new TH1F("deltaT116","Bar 16; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT116->GetYaxis()->SetTitle("counts");
TH1F *deltaT124 = new TH1F("deltaT124","Bar 24; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT124->GetYaxis()->SetTitle("counts");
TH1F *deltaT132 = new TH1F("deltaT132","Bar 32; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT132->GetYaxis()->SetTitle("counts");
TH1F *deltaT140 = new TH1F("deltaT140","Bar 40; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT140->GetYaxis()->SetTitle("counts");
TH1F *deltaT148 = new TH1F("deltaT148","Bar 48; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT148->GetYaxis()->SetTitle("counts");
TH1F *deltaT156 = new TH1F("deltaT156","Bar 56; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT156->GetYaxis()->SetTitle("counts");
TH1F *deltaT164 = new TH1F("deltaT164","Bar 64; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT164->SetMarkerStyle(7);
deltaT164->SetMarkerSize(7);
deltaT164->GetYaxis()->SetTitle("counts");

///~~~~~~~~~~~~~~
TH1F *deltaT216 = new TH1F("deltaT216","Bar 16; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT216->GetYaxis()->SetTitle("counts");
TH1F *deltaT224 = new TH1F("deltaT224","Bar 24; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT224->GetYaxis()->SetTitle("counts");
TH1F *deltaT232 = new TH1F("deltaT232","Bar 32; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT232->GetYaxis()->SetTitle("counts");
TH1F *deltaT240 = new TH1F("deltaT240","Bar 40; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT240->GetYaxis()->SetTitle("counts");
TH1F *deltaT248 = new TH1F("deltaT248","Bar 48; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT248->GetYaxis()->SetTitle("counts");
TH1F *deltaT256 = new TH1F("deltaT256","Bar 56; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT256->GetYaxis()->SetTitle("counts");
TH1F *deltaT264 = new TH1F("deltaT264","Bar 64; #Delta T (ns)  (ns)",100,-1.5,1.5);
deltaT264->SetMarkerStyle(7);
deltaT264->SetMarkerSize(7);
deltaT264->GetYaxis()->SetTitle("counts");

///---------------------------------------------------------------------------
///  Filling histograms
///---------------------------------------------------------------------------

for (ULong64_t j = 0 ; j < nentries ; j++){
    events->GetEntry(j);

    measuredVsTrueTimeRes16->Fill(deltaMeasured16-deltaTrue16);
    measuredVsTrueTimeRes24->Fill(deltaMeasured24-deltaTrue24);
    measuredVsTrueTimeRes32->Fill(deltaMeasured32-deltaTrue32);
    measuredVsTrueTimeRes40->Fill(deltaMeasured40-deltaTrue40);
    measuredVsTrueTimeRes48->Fill(deltaMeasured48-deltaTrue48);
    measuredVsTrueTimeRes56->Fill(deltaMeasured56-deltaTrue56);
    measuredVsTrueTimeRes64->Fill(deltaMeasured64-deltaTrue64);

    expectedTrueTimeRes16->Fill(deltaTtrueTimeResolution16-deltaTrue16);
    expectedTrueTimeRes24->Fill(deltaTtrueTimeResolution24-deltaTrue24);
    expectedTrueTimeRes32->Fill(deltaTtrueTimeResolution32-deltaTrue32);
    expectedTrueTimeRes40->Fill(deltaTtrueTimeResolution40-deltaTrue40);
    expectedTrueTimeRes48->Fill(deltaTtrueTimeResolution48-deltaTrue48);
    expectedTrueTimeRes56->Fill(deltaTtrueTimeResolution56-deltaTrue56);
    expectedTrueTimeRes64->Fill(deltaTtrueTimeResolution64-deltaTrue64);

    differenceTimeHis16->Fill(deltaTtrueTimeResolution16-deltaMeasured16);
    differenceTimeHis24->Fill(deltaTtrueTimeResolution24-deltaMeasured24);
    differenceTimeHis32->Fill(deltaTtrueTimeResolution32-deltaMeasured32);
    differenceTimeHis40->Fill(deltaTtrueTimeResolution40-deltaMeasured40);
    differenceTimeHis48->Fill(deltaTtrueTimeResolution48-deltaMeasured48);
    differenceTimeHis56->Fill(deltaTtrueTimeResolution56-deltaMeasured56);
    differenceTimeHis64->Fill(deltaTtrueTimeResolution64-deltaMeasured64);

    deltaT116->Fill(experimentalTimeMeasured16-deltaTtrueTimeResolution16);
    deltaT124->Fill(experimentalTimeMeasured24-deltaTtrueTimeResolution24);
    deltaT132->Fill(experimentalTimeMeasured32-deltaTtrueTimeResolution32);
    deltaT140->Fill(experimentalTimeMeasured40-deltaTtrueTimeResolution40);
    deltaT148->Fill(experimentalTimeMeasured48-deltaTtrueTimeResolution48);
    deltaT156->Fill(experimentalTimeMeasured56-deltaTtrueTimeResolution56);
    deltaT164->Fill(experimentalTimeMeasured64-deltaTtrueTimeResolution64);



    deltaT216->Fill(experimentalTimeMeasured16-deltaMeasured16);
    deltaT224->Fill(experimentalTimeMeasured24-deltaMeasured24);
    deltaT232->Fill(experimentalTimeMeasured32-deltaMeasured32);
    deltaT240->Fill(experimentalTimeMeasured40-deltaMeasured40);
    deltaT248->Fill(experimentalTimeMeasured48-deltaMeasured48);
    deltaT256->Fill(experimentalTimeMeasured56-deltaMeasured56);
    deltaT264->Fill(experimentalTimeMeasured64-deltaMeasured64);

   /* H2DBeforeCorrections->Fill(7,deltaTrue16-deltaMeasured16);
    H2DBeforeCorrections->Fill(6,deltaTrue24-deltaMeasured24);
    H2DBeforeCorrections->Fill(5,deltaTrue32-deltaMeasured32);
    H2DBeforeCorrections->Fill(4,deltaTrue40-deltaMeasured40);
    H2DBeforeCorrections->Fill(3,deltaTrue48-deltaMeasured48);
    H2DBeforeCorrections->Fill(2,deltaTrue56-deltaMeasured56);
    H2DBeforeCorrections->Fill(1,deltaTrue64-deltaMeasured64);*/


}
///==============================================================
///Fits
///==============================================================
//TF1 *f0 = new TF1("");
measuredVsTrueTimeRes16->Fit("landau");
measuredVsTrueTimeRes24->Fit("landau");
measuredVsTrueTimeRes32->Fit("landau");
measuredVsTrueTimeRes40->Fit("landau");
measuredVsTrueTimeRes48->Fit("landau");
measuredVsTrueTimeRes56->Fit("landau");
measuredVsTrueTimeRes64->Fit("landau");


expectedTrueTimeRes16->Fit("gaus");
expectedTrueTimeRes24->Fit("gaus");
expectedTrueTimeRes32->Fit("gaus");
expectedTrueTimeRes40->Fit("gaus");
expectedTrueTimeRes48->Fit("gaus");
expectedTrueTimeRes56->Fit("gaus");
expectedTrueTimeRes64->Fit("gaus");

TF1 *g1 = new TF1("g1","gaus",-0.15,0.15);
TF1 *g2 = new TF1("g2","gaus");
g2->SetLineColor(kBlue);


differenceTimeHis16->Fit(g1,"RQ","",-0.3,0.3);
differenceTimeHis24->Fit(g1,"RQ","",-0.3,0.3);
differenceTimeHis32->Fit(g1,"RQ","",-0.3,0.3);
differenceTimeHis40->Fit(g1,"RQ","",-0.3,0.3);
differenceTimeHis48->Fit(g1,"RQ","",-0.3,0.3);
differenceTimeHis56->Fit(g1,"RQ","",-0.3,0.3);
differenceTimeHis64->Fit(g1,"RQ","",-0.3,0.3);
//f1->SetLineColor(kRed);
deltaT116->Fit("g2");
deltaT124->Fit("g2");
deltaT132->Fit("g2");
deltaT140->Fit("g2");
deltaT148->Fit("g2");
deltaT156->Fit("g2");
deltaT164->Fit("g2");
deltaT216->Fit("gaus");
deltaT224->Fit("gaus");
deltaT232->Fit("gaus");
deltaT240->Fit("gaus");
deltaT248->Fit("gaus");
deltaT256->Fit("gaus");
deltaT264->Fit("gaus");

///==============================================================
generateCanvas1D(measuredVsTrueTimeRes16);
generateCanvas1D(measuredVsTrueTimeRes24);
generateCanvas1D(measuredVsTrueTimeRes32);
generateCanvas1D(measuredVsTrueTimeRes40);
generateCanvas1D(measuredVsTrueTimeRes48);
generateCanvas1D(measuredVsTrueTimeRes56);
generateCanvas1D(measuredVsTrueTimeRes64);
///==============================================================
generateCanvas1D(expectedTrueTimeRes16);
generateCanvas1D(expectedTrueTimeRes24);
generateCanvas1D(expectedTrueTimeRes32);
generateCanvas1D(expectedTrueTimeRes40);
generateCanvas1D(expectedTrueTimeRes48);
generateCanvas1D(expectedTrueTimeRes56);
generateCanvas1D(expectedTrueTimeRes64);
///==============================================================
generateCanvas1D(differenceTimeHis16);
generateCanvas1D(differenceTimeHis24);
generateCanvas1D(differenceTimeHis32);
generateCanvas1D(differenceTimeHis40);
generateCanvas1D(differenceTimeHis48);
generateCanvas1D(differenceTimeHis56);
generateCanvas1D(differenceTimeHis64);
///==============================================================
generateTwoHistogramCanvas(deltaT116,deltaT216);
generateTwoHistogramCanvas(deltaT124,deltaT224);
generateTwoHistogramCanvas(deltaT132,deltaT232);
generateTwoHistogramCanvas(deltaT140,deltaT240);
generateTwoHistogramCanvas(deltaT148,deltaT248);
generateTwoHistogramCanvas(deltaT156,deltaT256);
generateTwoHistogramCanvas(deltaT164,deltaT264);


}//program end












