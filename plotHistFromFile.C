#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLegend.h"


TH1F *loadHistFromFile(const char *file){
TTree *Tcharge = new TTree("Tcharge","Tcharge");
Tcharge->ReadFile(file,"lineNumber:charge");
Tcharge->Print();

TH1F *h2 = new TH1F("h2", "No Coil", 200, -0.3, 1);
float charge_noCoil;
ULong64_t nentries = (Int_t)Tcharge->GetEntries();

Tcharge->SetBranchAddress("charge", &charge_noCoil);

for (ULong64_t j = 0 ; j < nentries ; j++){
    Tcharge->GetEntry(j);
    h2->Fill(charge_noCoil);
}
Tcharge->ResetBranchAddresses();

TCanvas *c1 = new TCanvas();
//h2->Draw();
return h2;
}


void plotHistFromFile(){
 TH1F *a_histogram1= loadHistFromFile("noCoil_charge.dat");
// TH1F *a_histogram2= loadHistFromFile("processedWaveforms_coil.dat");
TH1F *a_histogram2= loadHistFromFile("coils_X0_Y0_box_charge.dat");
 TCanvas *c = new TCanvas ("c","A3",1000,700);
    TPad *pad1 = new TPad("pad1","",0,0,1,1);
    TPad *pad2 = new TPad("pad2","",0,0,1,1);
    pad2->SetFillStyle(4000);
    pad1->Draw();
    pad1->cd();
    // const char* a_textFileName = a_histogram1->GetName();
    // std::string a_nameFileStr = a_textFileName;
    // a_nameFileStr="temp/"+a_nameFileStr+".png";
    // const char* a_nameFile = a_nameFileStr.c_str();

    a_histogram1->Draw();
    a_histogram2->Draw("sames");
    pad1->SetLogy();   
    pad1->SetGrid();
    a_histogram1->Fit("gaus", "V","E1", 0.30, 0.65);
    a_histogram2->Fit("gaus", "V","E1", 0.30, 0.65);
    a_histogram1->GetFunction("gaus")->SetLineColor(kBlue);
    a_histogram1->SetLineWidth(3);
    a_histogram1->SetTitle("; Charge [pC] ;Counts");
    a_histogram1->GetXaxis()->SetTitleSize(.05);
    a_histogram1->GetYaxis()->SetTitleSize(.05);
    a_histogram1->GetYaxis()->SetTitleOffset(0.7);
    a_histogram1->GetXaxis()->SetTitleOffset(0.7);

    a_histogram2->GetFunction("gaus")->SetLineColor(kRed);
    c->Update();
    TPaveStats *ps1 = (TPaveStats*)a_histogram1->GetListOfFunctions()->FindObject("stats");
    ps1->SetX1NDC(0.7); ps1->SetX2NDC(0.9);
    ps1->SetTextColor(kBlue);
    ps1->Draw();
    pad1->Modified();
    a_histogram2->SetLineColor(kRed);
    a_histogram2->SetLineColor(kRed);
 
    c->Update();
    TPaveStats *ps2 = (TPaveStats*)a_histogram2->GetListOfFunctions()->FindObject("stats");
    gStyle->SetOptFit(100);
 //   gStyle->SetLabelSize(.6, "XY");
    ps2->SetX1NDC(0.4); ps2->SetX2NDC(0.6);
    ps2->SetTextColor(kRed);
    ps2->Draw();
    TLegend *leg =  new TLegend(0.65,0.45,0.9,0.55);
    //leg->SetHeader("");
    leg->AddEntry(a_histogram1,"No coil");
    leg->AddEntry(a_histogram2,"With Coil");
    leg->Draw();

  //  c->Update();
    //c->Print(a_nameFile);
    c->cd();

}





int main(){

//std::string inFile =      argv[1];     // Nombre del archivo
//TH1F *h=plotHistFromFile("processedWaveforms.dat");
plotHistFromFile();
return 0;
}

// int main(){
//     plotHistFromFile();

// }


