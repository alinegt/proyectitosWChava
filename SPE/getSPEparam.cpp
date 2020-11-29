#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TSpectrum.h"
#include "TMarker.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TString.h"

#include <stdlib.h>
#include "TMath.h"
#include "TF1.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
std::string base_name(std::string const & path)
{
  std::string base_filename= path.substr(path.find_last_of("/\\") + 1);
  std::string::size_type const p(base_filename.find_last_of('.'));
  return base_filename.substr(0, p);
}

// Function to load the histogram from dat file
TH1F *loadHistFromFile(const char *file){
TTree *Tcharge = new TTree("Tcharge","Tcharge");
Tcharge->ReadFile(file,"lineNumber:charge");
Tcharge->Print();

TH1F *PeakToValleyFit = new TH1F("PeakToValleyFit", "No Coil", 200, -0.3, 1);
float charge_noCoil;
ULong64_t nentries = (Int_t)Tcharge->GetEntries();

Tcharge->SetBranchAddress("charge", &charge_noCoil);

for (ULong64_t j = 0 ; j < nentries ; j++){
    Tcharge->GetEntry(j);
    PeakToValleyFit->Fill(charge_noCoil);
}
Tcharge->ResetBranchAddresses();

//TCanvas *c1 = new TCanvas();
//PeakToValleyFit->Draw();
return PeakToValleyFit;
}

  Double_t fitf(Double_t *x,Double_t *par) {
      Double_t arg = 0;
      Double_t arg1 =0;
      Double_t arg2 =0;
      Double_t arg3 =0;
      Double_t arg4_1 = 0;
      Double_t arg4 = 0;
       // par[0]= a
       // par[1]= b
       // par[2] = Npe
       // par[3] = sigma
       // par[4] = C
      arg = (par[0] * TMath::Exp (-par[1]*x[0]));
      arg2= par[2]*TMath::Exp(-par[2]);
      arg3 =  TMath::Sqrt(2*3.14159*1*TMath::Power( par[3],2 ));
      arg4_1=x[0]-par[4];
      arg4 = TMath::Exp( - (TMath::Power( arg4_1 , 2) )/ (  2*1*TMath::Power(par[3],2))  )      ;
      Double_t fitval = arg+arg2*arg3*arg4;
     //Double_t fitval = x[0]*par[0]+par[1] + par[2]+par[3]+ par[4];
      return fitval;
   } 




Double_t GetPeakToValley(char *argc){
 
TH1F *a_histogram1= loadHistFromFile(argc);
std::string filepath = argc;
std:: string filename= base_name(filepath);
char* filename_arr;
filename_arr = &filename[0];
   

 TCanvas *c = new TCanvas ("c","A3",1000,700);
    TPad *pad1 = new TPad("pad1","",0,0,1,1);

    pad1->Draw();
    pad1->cd();
    Double_t xliminf=0.07;
    Double_t xlimsup=0.4;
    a_histogram1->Draw();
    pad1->SetLogy();   
    pad1->SetGrid();
    a_histogram1->Fit("pol4", "SVM","E1", xliminf, xlimsup);
    
   // a_histogram1->GetFunction("gaus")->SetLineColor(kBlue);
    a_histogram1->SetLineWidth(3);
    a_histogram1->SetTitle(Form("%s; Charge [pC] ;Counts",filename_arr));
    a_histogram1->GetXaxis()->SetTitleSize(.05);
    a_histogram1->GetYaxis()->SetTitleSize(.05);
    a_histogram1->GetYaxis()->SetTitleOffset(0.7);
    a_histogram1->GetXaxis()->SetTitleOffset(0.7);
    a_histogram1->GetFunction("pol4")->SetLineWidth(5);


    TF1 *par = (TF1*) a_histogram1-> GetListOfFunctions()->FindObject("pol4");
    TF1 *func = new TF1("funcfit","pol4", xliminf, xlimsup);
    func-> SetParameters(par->GetParameter(0),par->GetParameter(1),par->GetParameter(2),par->GetParameter(3),par->GetParameter(4));
    func->Print();
    TSpectrum *s = new TSpectrum(2);
    TH1 *funcHist = func->GetHistogram();
    Int_t nfound = s->Search(funcHist,2,"goff",1);

    s->Print();
    printf("Found %d candidate peaks to fit\n",nfound);

    Double_t inflectionPoint =0;
    Double_t xx=0;
    // The slope will be negative for the first part
    // of the fitted curve, so finding the sign change
    // from negative to positive as an approximation of the minimum
    for (xx=xliminf;xx<xlimsup;xx+=0.001){
      inflectionPoint=func->Derivative(xx);
      if ( inflectionPoint>0 ){
    //        std::cout << xx << std::endl;
            break;

      }
    }
    Double_t xx2=0;
   for (xx2=xx;xx2<xlimsup;xx2+=0.001){
      inflectionPoint=func->Derivative(xx2);
      if ( inflectionPoint<0 ){
    //        std::cout << xx << std::endl;
            break;

      }
    }
    std::cout << func->Eval(xx) << std::endl;
    Double_t minimo= func->Eval(xx);
    Double_t maximo= func->Eval(xx2);
    Double_t *PeaksY = s->GetPositionY();
    Double_t *PeaksX = s->GetPositionX();

    Double_t firstPeakY = PeaksY[0]; 
    Double_t peak2Valley = firstPeakY/minimo;
    std::cout << firstPeakY << std::endl;
        std::cout << peak2Valley << std::endl;

 
// FIT Pedestal
   // TF1 *f1 = new TF1("f1", "gaus", -0.3, .2);
    TH1F *PedestalFit = (TH1F*)a_histogram1->Clone("PedestalFit");
    PedestalFit->Fit("gaus","","sames",-0.1,0.08);
    PedestalFit->GetFunction("gaus")->SetLineColor(kBlack);
    PedestalFit->GetFunction("gaus")->SetLineWidth(3);


    c->Update();
    TPaveStats *ps1 = (TPaveStats*)a_histogram1->GetListOfFunctions()->FindObject("stats");
    ps1->SetX1NDC(0.5); ps1->SetX2NDC(0.9);
    ps1->SetY1NDC(0.75); ps1->SetY2NDC(0.9);
    ps1->SetTextSize(.04);
    ps1->SetTextColor(kBlue);
    //ps1->SetOptStat(110);
    ps1->SetOptFit(100);

    ps1->Draw();
    pad1->Modified();
    TPaveStats *ps2 = (TPaveStats*)PedestalFit->GetListOfFunctions()->FindObject("stats");
    ps2->SetX1NDC(0.5); ps2->SetX2NDC(0.9);
    ps2->SetY1NDC(0.55); ps2->SetY2NDC(0.75);
    ps2->SetTextSize(.04);
    ps2->SetTextColor(kBlack);
    ps2->SetOptStat(1000000001);
    ps2->SetOptFit(0001);
    ps2->Draw();
    pad1->Modified(); 

   TMarker *mx = new TMarker(PeaksX[0],PeaksY[0],22);
    mx-> SetMarkerSize(2);
    mx-> SetMarkerColor(4);
 //   mx->Draw("SAME");
 
  TMarker *mn = new TMarker(xx,minimo,22);
    mn-> SetMarkerSize(2);
    mn-> SetMarkerColor(1);
    mn->Draw("SAME");
  
  TMarker *mx2 = new TMarker(xx2,maximo,22);
    mx2-> SetMarkerSize(2);
    mx2-> SetMarkerColor(3);
    mx2->Draw("SAME");
    c->Update();
    //gStyle->SetOptFit(101);
    // LEGEND
    TLegend *leg =  new TLegend(0.65,0.45,0.9,0.55);
    leg->AddEntry(a_histogram1,filename_arr);
   // leg->AddEntry(a_histogram1, );
  //  leg->Draw();
    c->cd();

    TLatex t(0.3,0.2,Form("Peak/valley:%g",peak2Valley));
    t.Draw();
    std::string outPath = "./data/plots/";

    c->Print( (outPath+filename+".png").c_str() );
    c->Close();
    std::cout << base_name(filepath) << std::endl;
    return peak2Valley;
}


int main(int argc, char **argv){

std::string inFile =      argv[1];     // Nombre del archivo
std::string outFile =  argv[2]; 
Double_t peak2valley= GetPeakToValley(argv[1]);
std::cout << peak2valley << std::endl;
std::string outFilePath = "/home/salvador/github/proyectitosWChava/SPE/data/SPEparam/"+outFile+".dat";
std::ofstream a_file;
a_file.open(outFilePath,std::ios::out | std::fstream::app);
a_file<< base_name(argv[1])<<" "<<peak2valley<<" "<<std::endl;
a_file.close();
return 0;
 
}