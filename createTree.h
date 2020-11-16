#ifndef _CREATE_TREE_H
#define _CREATE_TREE_H

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

//ROOT
#include "TTree.h"
#include "TFile.h"

using namespace std;

/* \class createTree
 * This class allow you to create a tree from the
 * raw data coming from the DRS4
 * */

class CreateTree
{
public:
    CreateTree();
    ~CreateTree();
    void createtree( char* infilename, char* outfilename ) ;
    string IntToString(int integer);
protected:
     std::vector<std::string> m_vNames;
     std::vector<float> m_var;
     std::vector<float> m_signal1;
     std::vector<float> m_signal2;
     ifstream a_inFile;
     div_t m_test;
     unsigned int m_index;
     unsigned int m_line;
     char* m_inFileName;

};
#endif
