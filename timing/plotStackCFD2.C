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
      
    TFile *f0 = new TFile("./data/rootFiles/mergeRoot.root");
   TH1F * h_shielding = (TH1F*)f0->Get("h_noCoil_feb8_LE");
   TH1F * h_noCoil = (TH1F*)f0->Get("h_noCoil_8feb_2_LE");
   TH1F *h_coilOn =  (TH1F*)f0->Get("h_coils_ON_AlumBox_X0_Y0_Z475_LE");
   
    TPaveStats *ps1 = (TPaveStats *)h_noCoil->GetListOfFunctions()->FindObject("stats");
    ps1->SetTextColor(kRed);
    ps1->SetX1NDC(0.15);
    ps1->SetX2NDC(0.4);
    ps1->SetY1NDC(0.6);
    ps1->SetY2NDC(0.8);
    h_noCoil->SetLineColor(kRed);
    h_noCoil->SetLineWidth(3);
    hs->Add(h_noCoil);
    
     TPaveStats *ps2 = (TPaveStats *)h_shielding->GetListOfFunctions()->FindObject("stats");
    ps2->SetTextColor(kBlue);
    ps2->SetX1NDC(0.15);
      ps2->SetX2NDC(0.4);
      ps2->SetY1NDC(0.35);
      ps2->SetY2NDC(0.55);
     h_shielding->SetLineColor(kBlue);
     h_shielding->SetLineWidth(3);

     hs->Add(h_shielding);


  TPaveStats *ps3 = (TPaveStats *)h_coilOn->GetListOfFunctions()->FindObject("stats");
       ps3->SetTextColor(kBlack);
    ps3->SetX1NDC(0.6);
      ps3->SetX2NDC(0.85);
      ps3->SetY1NDC(0.35);
      ps3->SetY2NDC(0.55);
    h_coilOn->SetLineColor(kBlack);
    h_coilOn->SetLineWidth(3);

        // hs->Add(h_coilOn);

    hs->SetTitle(Form("; Time [ns] ; Counts"));

     THStack *hs_res = new THStack("hs_res","");

  //  TH1F residual_coilOn=   *h_noCoil- *h_coilOn; //  *h_coilOn - *h_noCoil;  
   TH1F residual_shielding= *h_noCoil - *h_shielding;//  *h_coilOn - *h_shielding ;
//   residual_coilOn.SetTitle("coilON - no Coil");
 //  residual_shielding.SetTitle("coilON - coil_shielding");

   //residual_coilOn.SetTitle("no Coil - coil ON");
   residual_shielding.SetTitle("no Coil_morning - no Coil_afternoon");

  // residual_coilOn.SetLineColor(kBlue);
   residual_shielding.SetLineColor(kBlack);



  //  hs_res-> Add(&residual_coilOn);
   hs_res-> Add(&residual_shielding);
      hs_res->SetTitle(Form("; Time [ns] ; Counts"));

   
   TCanvas *cs = new TCanvas("cs","cs",300,2000,1500,2000);
      TText T; T.SetTextFont(42); T.SetTextAlign(21);

   cs->Divide(1,2);

   cs->cd(1); hs->Draw("nostack"); T.DrawTextNDC(.5,.95,"Timing, Leading edge");

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