
// #include "TCanvas.h"
// #include "TROOT.h"
// #include "TGraphErrors.h"
// #include "TF1.h"
// #include "TLegend.h"
// #include "TArrow.h"
// #include "TLatex.h"
// #include "TFile.h"

void plotCFD_intermediateWF_spe() {

TFile *f=new TFile("cfdResults.root");
TTree *Tree=(TTree*)f->Get("Tree");
Tree->Print();

Tree->Draw("resultSignal_ch1:Iteration$","Entry$<50");
//gPad->GetListOfPrimitives()->Print(); // print what’s available
//Tree->Draw("resultSignal_ch2:Iteration$","Entry$==0", "same");
//Tree->Draw("ch1:Iteration$","Entry$==0", "same");
//Tree->Draw("ch2:Iteration$","Entry$==0", "same");
// Tree->Scan("timeDifference");
// Tree->Scan("timech2","Entry$==0");
// Tree->Scan("timech1","Entry$==0");
//TLine *lch2 = new TLine(29.179487,-200,29.179487,200);
//lch2->Draw();
//TLine *lch1 = new TLine(53.257576,-200,53.257576,200);
//lch1->Draw();


}


int main(){
    plotCFD_intermediateWF_spe();
    }