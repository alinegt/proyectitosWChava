#include "createTree.h"

using namespace std;

///Constructor
CreateTree::CreateTree() {}
///Desctructor
CreateTree::~CreateTree() {}

///*******************************************************
/// Converts an int to a string
string CreateTree::IntToString(int integer) {
  ostringstream stst;
  stst << integer;
  return stst.str();
}
///*******************************************************
void CreateTree::createtree( char* a_inFileName, char* a_outFileName ) {
 m_inFileName=a_inFileName;

 a_inFile.open(a_inFileName);
 //m_vNames.push_back("motorPosition");
 //m_vNames.push_back("localEventNumber");
 //m_vNames.push_back("channel");
 //m_vNames.push_back("getTriggerCell");
 //m_vNames.push_back("timeStamp");
 //m_vNames.push_back("Temperature");
for(int i = 0 ; i < 70; i++){
 m_vNames.push_back("signal"+IntToString(i));
}
  m_var.resize(m_vNames.size(), 0);

  // create a new ROOT file and write TTree into it
  TFile *m_outfile = new TFile(a_outFileName,"RECREATE");
  // create a TTree
  TTree *tree = new TTree("T",m_inFileName);



 //Create branches for the headers
  //for ( unsigned int i=0; i<6; i++ ) {
  //  std::string format = m_vNames[i]+"/F";
  //  tree->Branch(m_vNames[i].c_str(),&(m_var[i]),format.c_str());
  //}

  tree->Branch("ch1",&m_signal1);
  tree->Branch("ch2",&m_signal2);
  m_signal1.clear();
  m_signal2.clear();

  // fill the tree from the values in ASCII file 
  m_index=0;
  m_line=1;
  //string dummyLine;
  //getline (a_inFile, dummyLine);
  while ( !a_inFile.eof() ) {
    a_inFile >> m_var[m_index];
    m_index++;
    if ( m_index == m_vNames.size() ) {
        m_test=div(m_line,2);
        if(m_test.rem>0){
            for ( unsigned int k=0; k<m_vNames.size(); ++k){
                  m_signal1.push_back(m_var[k]);
                }
        }
        else{
            for ( unsigned int k=0; k<m_vNames.size(); ++k){
                  m_signal2.push_back(m_var[k]);
                }

            tree->Fill();
            m_signal1.clear();
            m_signal2.clear();
        }

      m_index=0;
      m_line++;
    }
  }  
  tree->Write();
  tree->Print();

  // close files
  a_inFile.close();
  m_outfile->Close();

  return;
}

int main(int argc, char **argv) {
    CreateTree create;
if (argc == 3) {
    std::cout << std::endl;
    std::cout << "createtree: reading ASCII file " << argv[1] << " and writing ROOT file " << argv[2] << std::endl;
    std::cout << std::endl;
    create.createtree( argv[1], argv[2] );

  } else {
    std::cout << "ERROR: two arguments required" << std::endl;
    std::cout << "Example: " << std::endl;
    std::cout << "./createTreeData /path/to/input.dat /path/to/output.root" << std::endl;
    return 1;
  }

  return 0;
}

