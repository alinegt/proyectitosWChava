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

void plotHist(){


TCanvas *c = new TCanvas("c", "A3", 1000, 700);
   TFile *f0 = new TFile("./data/rootFiles/coils_X0_Y0_Z0.root");
    TH1F * h_coils_X0_Y0_Z0 = (TH1F*)f0->Get("htemp");
    h_coils_X0_Y0_Z0->Draw();
   c->Print("./data/plots/h_coils_X0_Y0_Z0.png");
   c->Close();

  TCanvas *c1 = new TCanvas("c", "A3", 1000, 700);
   TFile *f475 = new TFile("./data/rootFiles/coils_X0_Y0_Z475.root");
    TH1F * h_coils_X0_Y0_Z475 = (TH1F*)f475->Get("htemp");
    h_coils_X0_Y0_Z475->Draw();
   c1->Print("./data/plots/h_coils_X0_Y0_Z475.png");
   c1->Close();
TCanvas *c2 = new TCanvas("c", "A3", 1000, 700);
   TFile *f = new TFile("./data/rootFiles/noCoil.root");
    TH1F * h_noCoil= (TH1F*)f->Get("htemp");
    h_noCoil->Draw();
   c2->Print("./data/plots/h_noCoil.png");
   c2->Close();
TCanvas *c3 = new TCanvas("c", "A3", 1000, 700);
    TTree * t_noCoil= (TTree*)f->Get("Tree");
  //  t_noCoil->Draw();
TH1F *hframe = new TH1F("hframe","CFD Results",100,0,200);
hframe->Draw();
 hframe->GetYaxis()->SetRangeUser(-.13, .1);
// hframe->GetXaxis()->SetRangeUser(0., 110.);  

    t_noCoil->Draw("resultSignal_ch1:Iteration$","Entry$<1", "same");
    t_noCoil->Draw("delaySignal_ch1:Iteration$","Entry$<1", "same");

    t_noCoil->Draw("ch1:Iteration$","Entry$<1","same");
 c->SetGrid();

  // c3->Print("./data/plots/h_noCoil.png");
  // c3->Close();
  
    // h_coils_X0_Y0_Z475->Draw();
    // h_noCoil->Draw();
  // c->Close();


}

int main(){

plotHist();



}