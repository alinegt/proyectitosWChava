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
   //   Double_t arg1 =0;
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




Double_t GetPeakToValley(char *argc, double_t *peak2Valley, double_t *sigma_fit){
 
TH1F *h_peakToValley= loadHistFromFile(argc);
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
    h_peakToValley->Draw();
    pad1->SetLogy();   
    pad1->SetGrid();
///////////////////////////////////////////////////////////////////////
    // Fit polynomial to find peak and valley values
///////////////////////////////////////////////////////////////////////
    h_peakToValley->Fit("pol4", "SM","E1", xliminf, xlimsup);
   // h_peakToValley->GetFunction("gaus")->SetLineColor(kBlue);
    h_peakToValley->SetLineWidth(3);
    h_peakToValley->SetTitle(Form("%s; Charge [pC] ;Counts",filename_arr));
    h_peakToValley->GetXaxis()->SetTitleSize(.05);
    h_peakToValley->GetYaxis()->SetTitleSize(.05);
    h_peakToValley->GetYaxis()->SetTitleOffset(0.7);
    h_peakToValley->GetXaxis()->SetTitleOffset(0.7);
    h_peakToValley->GetFunction("pol4")->SetLineWidth(5);
    // Extracting function from fitting
    

///////////////////////////////////////////////////////////////////////  
   // New function to get the data from the fit
///////////////////////////////////////////////////////////////////////
    TF1 *par = (TF1*) h_peakToValley-> GetListOfFunctions()->FindObject("pol4");
    
    TF1 *func = new TF1("funcfit","pol4", xliminf, xlimsup);
    Double_t parameters[5];
    par->GetParameters(&parameters[0]);
    std::cout << parameters << std::endl;
    func->SetParameters(parameters);
    const  Double_t errors[5]= {par->GetParError(0),par->GetParError(1),par->GetParError(2),par->GetParError(3),par->GetParError(4)};
    func-> SetParErrors(errors);
    //func->SetLineColor(kViolet);
    func->Draw("SAME");

    //func->Print();
    Double_t inflectionPoint =0;
    Double_t xx=0;
    // The slope will be negative for the first part
    // of the fitted curve, so finding the sign change
    // from negative to positive as an approximation fo the minimum which would be the valley
    for (xx=xliminf;xx<xlimsup;xx+=0.001){
      inflectionPoint=func->Derivative(xx);
      if ( inflectionPoint>0 ){
            break;

      }
    }
///////////////////////////////////////////////////////////////////////
    // Once the valley was found, the next sign change would be
    // a maximum, the peak of the SPE 
///////////////////////////////////////////////////////////////////////
    Double_t xx2=0;
   for (xx2=xx;xx2<xlimsup;xx2+=0.001){
      inflectionPoint=func->Derivative(xx2);
      if ( inflectionPoint<0 ){
            break;

      }
    }
///////////////////////////////////////////////////////////////////////
// Estimating peak/valley 
///////////////////////////////////////////////////////////////////////
    Double_t valley= func->Eval(xx);
    Double_t peak= func->Eval(xx2);
    *peak2Valley = peak/valley;
    std::cout << peak2Valley << std::endl;

///////////////////////////////////////////////////////////////////////
   // Alternative method to find the peaks using TSpectrum function
///////////////////////////////////////////////////////////////////////

//    TSpectrum *s = new TSpectrum(2);
//    TH1 *funcHist = func->GetHistogram();
//    Int_t nfound = s->Search(funcHist,2,"goff",1);
//    Double_t *PeaksY = s->GetPositionY();
//    Double_t *PeaksX = s->GetPositionX();
 //   Double_t firstPeakY = PeaksY[0]; 
    //s->Print();
    //printf("Found %d candidate peaks to fit\n",nfound);



/////////////////////////////////////////////////////////////////////// 
////// FIT Pedestal
///////////////////////////////////////////////////////////////////////

   // TF1 *f1 = new TF1("f1", "gaus", -0.3, .2);
    TH1F *h_pedestalFit = (TH1F*)h_peakToValley->Clone("h_pedestalFit");
    h_pedestalFit->Fit("gaus","","sames",-0.1,0.08);
    h_pedestalFit->GetFunction("gaus")->SetLineColor(kBlack);
    h_pedestalFit->GetFunction("gaus")->SetLineWidth(3);
    TF1 *parGaus = (TF1*) h_pedestalFit-> GetListOfFunctions()->FindObject("gaus");

    *sigma_fit = parGaus->GetParameter(2);


    c->Update();
    TPaveStats *ps1 = (TPaveStats*)h_peakToValley->GetListOfFunctions()->FindObject("stats");
    ps1->SetX1NDC(0.5); ps1->SetX2NDC(0.9);
    ps1->SetY1NDC(0.75); ps1->SetY2NDC(0.9);
    ps1->SetTextSize(.04);
    ps1->SetTextColor(kBlue);
    //ps1->SetOptStat(110);
    ps1->SetOptFit(100);

    ps1->Draw();
    pad1->Modified();
    TPaveStats *ps2 = (TPaveStats*)h_pedestalFit->GetListOfFunctions()->FindObject("stats");
    ps2->SetX1NDC(0.5); ps2->SetX2NDC(0.9);
    ps2->SetY1NDC(0.55); ps2->SetY2NDC(0.75);
    ps2->SetTextSize(.04);
    ps2->SetTextColor(kBlack);
    ps2->SetOptStat(1000000001);
    ps2->SetOptFit(0001);
    ps2->Draw();
    pad1->Modified(); 

  
 ///////////////////////////////////////////////////////////////////////
    // LEGENDS
///////////////////////////////////////////////////////////////////////
    TLegend *leg =  new TLegend(0.65,0.45,0.9,0.55);
    leg->AddEntry(h_peakToValley,filename_arr);
   // leg->AddEntry(a_histogram1, );
  //  leg->Draw();
///////////////////////////////////////////////////////////////////////
/////// Markers for minimum and maximum from fitted polynomial ////////
///////////////////////////////////////////////////////////////////////

//    TMarker *mx = new TMarker(PeaksX[0],PeaksY[0],22);
//     mx-> SetMarkerSize(2);
//     mx-> SetMarkerColor(4);
//  //   mx->Draw("SAME");
 
  TMarker *mn = new TMarker(xx,valley,22);
    mn-> SetMarkerSize(2.5);
    mn-> SetMarkerColor(kGreen);
    mn->Draw("SAME");
  
  TMarker *mx2 = new TMarker(xx2,peak,22);
    mx2-> SetMarkerSize(2.5);
    mx2-> SetMarkerColor(kGreen);
    mx2->Draw("SAME");
  
  c->Update();




    c->cd();
///////////////////////////////////////////////////////////////////////
///// Text box
///////////////////////////////////////////////////////////////////////
    TLatex t(0.3,0.2,Form("Peak/valley:%g",*peak2Valley));
    t.Draw();
    std::string outPath = "./data/plots/";
    c->Print( (outPath+filename+".png").c_str() );
    c->Close();
    std::cout << base_name(filepath) << std::endl;


    return 0;

}


int main(int argc, char **argv){
Double_t peaktovalley, sigma_fit;
std::string inFile =      argv[1];     // Nombre del archivo
std::string outFile =  argv[2]; 
GetPeakToValley(argv[1], &peaktovalley, &sigma_fit );
std::cout << peaktovalley << std::endl;
std::string outFilePath = "/home/salvador/github/proyectitosWChava/SPE/data/SPEparam/"+outFile+".dat";
std::ofstream a_file;
a_file.open(outFilePath,std::ios::out | std::fstream::app);
a_file<< base_name(argv[1])<<" "<< peaktovalley <<" "<< sigma_fit<<" "<< std::endl;
a_file.close();
return 0;
 
}