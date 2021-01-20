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
void plotStackLE() {
   

    THStack *hs = new THStack("hs","");

    TFile *f0 = new TFile("./data/rootFiles/coils_X0_Y0_Z0.root");
    TH1F * h_coils_X0_Y0_Z0 = (TH1F*)f0->Get("h_coils_X0_Y0_Z0_LE;1");
   // h_coils_X0_Y0_Z0->Draw();
    h_coils_X0_Y0_Z0->SetLineColor(kRed);
        h_coils_X0_Y0_Z0->SetLineWidth(3);

    hs->Add(h_coils_X0_Y0_Z0);
    
    TFile *f475 = new TFile("./data/rootFiles/coils_X0_Y0_Z475.root");
    TH1F * h_coils_X0_Y0_Z475 = (TH1F*)f475->Get("h_coils_X0_Y0_Z475_LE;1");
    //h_coils_X0_Y0_Z475->Draw();
    h_coils_X0_Y0_Z475->SetLineColor(kBlue);
    h_coils_X0_Y0_Z475->SetLineWidth(3);

    hs->Add(h_coils_X0_Y0_Z475);

    TFile *f = new TFile("./data/rootFiles/noCoil.root");
    TH1F * h_noCoil = (TH1F*)f->Get("h_noCoil_LE;1");
   // h_noCoil->Draw();
    h_noCoil->SetLineColor(kBlack);
    h_noCoil->SetLineWidth(3);
        hs->Add(h_noCoil);


   TFile *fsh = new TFile("./data/rootFiles/coils_sh_2_X0_Y0_Z475.root");
    TH1F * h_coils_sh_X0_Y0_Z475 = (TH1F*)fsh->Get("h_coils_sh_2_X0_Y0_Z475_LE;1");
   // h_coils_X0_Y0_Z0->Draw();
    h_coils_sh_X0_Y0_Z475->SetLineColor(kViolet);
        h_coils_sh_X0_Y0_Z475->SetLineWidth(3);

    hs->Add(h_coils_sh_X0_Y0_Z475);
    


   TCanvas *cs = new TCanvas("cs","cs",300,2000,1500,2000);
   TText T; T.SetTextFont(42); T.SetTextAlign(21);
   
    THStack *hs_res = new THStack("hs_res","");

   TH1F residual_X0_Y0_Z0=   *h_coils_X0_Y0_Z0   - *h_noCoil;  
   TH1F residual_X0_Y0_Z475=  *h_coils_X0_Y0_Z475 - *h_noCoil ;
   TH1F residual_sh_X0_Y0_Z475=  *h_coils_sh_X0_Y0_Z475 - *h_noCoil ;

   residual_X0_Y0_Z0.SetLineColor(kRed);
   residual_X0_Y0_Z475.SetLineColor(kBlack);
   residual_sh_X0_Y0_Z475.SetLineColor(kViolet);


   hs_res-> Add(&residual_X0_Y0_Z0);
   hs_res-> Add(&residual_X0_Y0_Z475);
   hs_res-> Add(&residual_sh_X0_Y0_Z475);




   cs->Divide(1,2);

   cs->cd(1); hs->Draw("nostack"); T.DrawTextNDC(.5,.95,"Timing");
   cs->Modified();   
   gPad->BuildLegend(0.75,0.75,0.95,0.95,"");

   cs->cd(2); hs_res->Draw("nostack");T.DrawTextNDC(.5,.95,"Residuals");
   cs->Modified();
   gPad->BuildLegend(0.75,0.75,0.95,0.95,"");


 //  cs->cd(3); hs->Draw("nostackb"); T.DrawTextNDC(.5,.95,"Option \"nostackb\"");
 //  cs->cd(4); hs->Draw("lego1"); T.DrawTextNDC(.5,.95,"Option \"lego1\"");
   cs->Print("stack_LE.png");
   //cs->Close();
   return cs;

  cs->Close();
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

plotStackLE();



}