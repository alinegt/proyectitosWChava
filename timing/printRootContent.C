void printRootContent(char* rootFile) {

    TFile *f0 = new TFile(rootFile);
    f0->GetListOfKeys()->Print();
    gROOT->ProcessLine(".q");
}
