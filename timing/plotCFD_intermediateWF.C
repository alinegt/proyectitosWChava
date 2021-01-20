
// #include "TCanvas.h"
// #include "TROOT.h"
// #include "TGraphErrors.h"
// #include "TF1.h"
// #include "TLegend.h"
// #include "TArrow.h"
// #include "TLatex.h"
// #include "TFile.h"

void plotCFD_intermediateWF() {
TFile *f=new TFile("./data/rootFiles/coils_X0_Y0_Z0.root");
TTree *Tree=(TTree*)f->Get("Tree");
Tree->Print();
 TMultiGraph *mg = new TMultiGraph();

int n =Tree->Draw("resultSignal_ch1:Iteration$","Entry$==0", "goff");
TGraph *g = new TGraph(n,Tree->GetV2(),Tree->GetV1());
//axis->SetLimits(-10,10);
// g->Draw("AC*");
g->SetMinimum(-0.15);
g->SetMaximum(0.15);
g->SetMarkerStyle(25);
g->SetMarkerSize(0.5);

//
int n1 = Tree->Draw("ch1:Iteration$","Entry$==0", "goff");
TGraph *g1 = new TGraph(n1,Tree->GetV2(),Tree->GetV1());
g1->SetLineStyle(3);
g1->SetMarkerStyle(2);
// g1->SetMarkerSize();
g1->SetMarkerSize(0.5);

// g1->Draw("same");
g1->SetMinimum(-0.15);
g1->SetMaximum(0.15);

int n2 = Tree->Draw("delaySignal_ch1:Iteration$","Entry$==0", "goff");
TGraph *g2 = new TGraph(n2,Tree->GetV2(),Tree->GetV1());
g2->SetLineStyle(3);
g2->SetMarkerSize(0.5);
g2->SetMarkerStyle(kCircle);
// g2->Draw("same");
   mg->Add(g);
   mg->Add(g1);
   mg->Add(g2);
   mg->Draw("ACP");
//Tree->Print("timeCh1","Entry$==51");
//Tree->Draw("delaySignal_ch1:Iteration$","Entry$==0", "same");


//htemp->SetLineColor(kRed);

// Tree->Draw("ch2:Iteration$","Entry$==0", "same");
// Tree->Scan("timeDifference");
// Tree->Scan("timech2","Entry$==0");
// Tree->Scan("timech1","Entry$==0");
TLine *lch2 = new TLine(29.179487,-200,29.179487,200);
lch2->Draw();
TLine *lch1 = new TLine(53.257576,-200,53.257576,200);
lch1->Draw();


}


int main(){
    plotCFD_intermediateWF();
    }