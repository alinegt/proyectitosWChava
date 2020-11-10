#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"

void plotHistFromFile(void){

TTree *Tcharge = new TTree("Tcharge","Tcharge");
Tcharge->ReadFile("processedWaveforms.dat","lineNumber:charge");
Tcharge->Draw("-charge/(1e-12)");
Tcharge->Print();
}

int main(){
    plotHistFromFile();
}