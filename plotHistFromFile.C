#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"

void plotHistFromFile(void){

TTree *Tcharge = new TTree("Tcharge","Tcharge");
Tcharge->ReadFile("processedWaveforms.dat","lineNumber:charge");
//Tcharge->Draw("charge");
Tcharge->Print();

TTree *Tcharge_coil = new TTree("Tcharge_coil","Tcharge_coil");
Tcharge_coil->ReadFile("processedWaveforms_coil.dat","lineNumber:charge_coil");
//Tcharge_coil->Draw("charge");
Tcharge_coil->Print();

TH1F *h1 = new TH1F("h1", "h1 title", 169, 0, 1);
//h1->Fill;
//THStack *hs = new THStack("hs","stack example");
//hs->Add(h1);
Tcharge->Draw("charge>>h1","","");
//gPad->GetListOfPrimitives()->Print(); 
//gPad->GetListOfPrimitives()->At(0);
//TFrame ((TH1F*)(gPad->GetListOfPrimitives()->At(0)))->SetLineColor(1); 
Tcharge_coil->Draw("charge_coil","", "same");
//Tcharge->Draw("charge>>h1");
//Tcharge_coil->Draw("charge_coil>>h1");
h1->SetLineColor(2);
}

int main(){
    plotHistFromFile();

}


