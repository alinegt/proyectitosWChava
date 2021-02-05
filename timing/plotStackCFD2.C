#include <iostream>
#include <string>
#include <cstring>

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <algorithm> // std::min_element
//#include <iterator>  // std::begin, std::end
void plotStackCFD2() {
   

    THStack *hs = new THStack("hs","");

    TFile *f0 = new TFile("./data/rootFiles/Coils_OFF_AlumBox_X0_Y0_Z475.root");
    TH1F * h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD = (TH1F*)f0->Get("h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD;1");
     TPaveStats *ps1 = (TPaveStats *)h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD->GetListOfFunctions()->FindObject("stats");
    ps1->SetTextColor(kRed);
    ps1->SetX1NDC(0.15);
      ps1->SetX2NDC(0.4);
      ps1->SetY1NDC(0.6);
      ps1->SetY2NDC(0.8);
    h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD->SetLineColor(kRed);
        h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD->SetLineWidth(3);

    hs->Add(h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD);
    
    TFile *f475 = new TFile("./data/rootFiles/Coils_ON_AlumBox_X0_Y0_Z475.root");
    TH1F * h_Coils_ON_AlumBox_X0_Y0_Z475_CFD = (TH1F*)f475->Get("h_Coils_ON_AlumBox_X0_Y0_Z475_CFD;1");
     TPaveStats *ps2 = (TPaveStats *)h_Coils_ON_AlumBox_X0_Y0_Z475_CFD->GetListOfFunctions()->FindObject("stats");
    ps2->SetTextColor(kBlue);
    ps2->SetX1NDC(0.15);
      ps2->SetX2NDC(0.4);
      ps2->SetY1NDC(0.35);
      ps2->SetY2NDC(0.55);
    h_Coils_ON_AlumBox_X0_Y0_Z475_CFD->SetLineColor(kBlue);
    h_Coils_ON_AlumBox_X0_Y0_Z475_CFD->SetLineWidth(3);

    hs->Add(h_Coils_ON_AlumBox_X0_Y0_Z475_CFD);

    TFile *f = new TFile("./data/rootFiles/Coils_ON_muMetal2_X0_Y0_Z475.root");
    TH1F * h_Coils_ON_muMetal2_X0_Y0_Z475_CFD = (TH1F*)f->Get("h_Coils_ON_muMetal2_X0_Y0_Z475_CFD;1");
  TPaveStats *ps3 = (TPaveStats *)h_Coils_ON_muMetal2_X0_Y0_Z475_CFD->GetListOfFunctions()->FindObject("stats");
       ps3->SetTextColor(kBlack);
    ps3->SetX1NDC(0.6);
      ps3->SetX2NDC(0.85);
      ps3->SetY1NDC(0.35);
      ps3->SetY2NDC(0.55);
    h_Coils_ON_muMetal2_X0_Y0_Z475_CFD->SetLineColor(kBlack);
    h_Coils_ON_muMetal2_X0_Y0_Z475_CFD->SetLineWidth(3);

        hs->Add(h_Coils_ON_muMetal2_X0_Y0_Z475_CFD);

   hs->SetTitle(Form("; Time [ns] ; Counts"));

    THStack *hs_res = new THStack("hs_res","");

   TH1F residual_Coils_ON_AlumBox_X0_Y0_Z475_CFD=   *h_Coils_ON_AlumBox_X0_Y0_Z475_CFD   - *h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD;  
   TH1F residual_Coils_ON_muMetal2_X0_Y0_Z475_CFD=  *h_Coils_ON_muMetal2_X0_Y0_Z475_CFD - *h_Coils_OFF_AlumBox_X0_Y0_Z475_CFD ;

   residual_Coils_ON_AlumBox_X0_Y0_Z475_CFD.SetLineColor(kBlue);
   residual_Coils_ON_muMetal2_X0_Y0_Z475_CFD.SetLineColor(kBlack);



   hs_res-> Add(&residual_Coils_ON_AlumBox_X0_Y0_Z475_CFD);
   hs_res-> Add(&residual_Coils_ON_muMetal2_X0_Y0_Z475_CFD);
      hs_res->SetTitle(Form("; Time [ns] ; Counts"));

   
   TCanvas *cs = new TCanvas("cs","cs",300,2000,1500,2000);
      TText T; T.SetTextFont(42); T.SetTextAlign(21);

   cs->Divide(1,2);

   cs->cd(1); hs->Draw("nostack"); T.DrawTextNDC(.5,.95,"Timing, CFD");

     hs->GetXaxis()->SetTitleOffset(0.5);
      hs->GetXaxis()->SetTitleSize(0.1);
    hs->GetYaxis()->SetTitleOffset(0.4);
      hs->GetYaxis()->SetTitleSize(0.1);
     hs->GetXaxis()->SetLabelSize(.06);
     hs->GetYaxis()->SetLabelSize(.06);


   cs->Modified();   
  //  gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
   gPad->SetGrid();
   cs->cd(2); hs_res->Draw("nostack");T.DrawTextNDC(.5,.95,"Residuals");

     hs_res->GetXaxis()->SetTitleOffset(0.5);
      hs_res->GetXaxis()->SetTitleSize(0.1);
        hs_res->GetYaxis()->SetTitleOffset(0.4);
      hs_res->GetYaxis()->SetTitleSize(0.1);
           hs_res->GetXaxis()->SetLabelSize(0.05);

           hs_res->GetYaxis()->SetLabelSize(0.05);

   cs->Modified();
   gPad->BuildLegend(0.6,0.65,0.95,0.95,"");
   gPad->SetGrid();

   cs->Modified();


 //  cs->cd(3); hs->Draw("nostackb"); T.DrawTextNDC(.5,.95,"Option \"nostackb\"");
 //  cs->cd(4); hs->Draw("lego1"); T.DrawTextNDC(.5,.95,"Option \"lego1\"");
   cs->Print("stack_CFD.png");
   //cs->Close();

  // cs->Close();
     return cs;

}

void plotHist(){


TCanvas *c = new TCanvas("c", "A3", 1000, 700);
   TFile *f00 = new TFile("./data/rootFiles/coils_X0_Y0_Z0.root");
    TH1F * h_coils_X0_Y0_Z0 = (TH1F*)f00->Get("htemp");
    h_coils_X0_Y0_Z0->Draw();
   c->Print("h_coils_X0_Y0_Z0.png");
    // h_coils_X0_Y0_Z475->Draw();
    // h_noCoil->Draw();
   c->Close();


}

int main(){

plotStackCFD2();



}