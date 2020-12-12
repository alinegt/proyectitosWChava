
//Este programa procesa los datos tomados por el sistema de adquisición de datos de CAEN, y se obtienen un archivo con datos de salida

//c++ includes
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TFile.h"

using namespace std;

int string_to_int(string _string){
   istringstream ss(_string);
   int number;
   ss >> number;
   return number;
}

float rms(vector <float> &noise){

	double sum = 0;
	for (int i = 0; i < (int)noise.size(); i++)
		sum += pow(noise[i], 2);
	return sqrt(sum / noise.size());
}





int main(int argc, char **argv)
{
   std::string inFile =      argv[1];     // Nombre del archivo
   std::string binInicial =  argv[2]; //Donde comienza a hacer el proceso
   std::string anchoPulso =  argv[3]; //Ancho del pulso aproximado
   std::string noMuestras =  argv[4]; // No. puntos en el pulso
   std::string noEventos =   argv[5]; // No. de eventos
   std::string deltaTiempo = argv[6]; // delta tiempo en s 
   std::string outDatFile = argv[7]; // 
   std::string outRootFile = argv[8]; //
   
/************************************************************************************************/
/****************************************VARIABLES MENU******************************************/
   float bin_inicial= std::stof(binInicial); 
   float ancho=std::stof(anchoPulso);
   float No_muestras= std::stof(noMuestras);       
   float No_eventos= std::stof(noEventos);      
   float delta_t= std::stof(deltaTiempo) ;           
   float R=50.0; // Resistencia en Ohms
/************************************************************************************************//************************************************************************************************/

   FILE *fichero;
   FILE *fichero2;
   unsigned int i,iprima,i2=1; 
   float x1=0, x2=0;//, x3=0, ch2=0, ch3, ch4;
   float carga=0, it=1;
   int voltaje_max=0, voltaje_min=0;
   float prom_max=0, prom_min=0;
   float t10=0, t50=0, t90=0, ct10=0, ct50=0, ct90=0;
   float Carga_actual=0, Carga_anterior=0, tiempo=1;
  // char c1[10],c2[10],c3[10],c4[10],c5[10];
   float sum_ruido1; // Sumatoria en primer parte de la waveform
   float sum_ruido2; // Sumatoria en segunda parte de la waveform despues del pulso
   float sum_volt;   // Sumatoria en el rango del pulso
   float volt;    // Valor de voltaje en osciloscopio en Volts
   float avg_ruido1; // Sumatoria del ruido antes de pulso dividida por el no. de muestras en ese rango
   float avg_ruido2; // Sumatoria del ruido despues de pulso dividida por el no. de muestras en ese rango
   fichero = fopen( inFile.c_str(), "r" );
   fichero2 = fopen( inFile.c_str(), "r" );
   float picoScale = 1e-12; //  factor to scale to picoCoulombs
   float flipConstant = -1;  // Negative pulses flipped by this constant
   float amplifGain = 10;
   std::vector<float> v_noise;
   gROOT->ProcessLine("#include <vector>"); 
   std::vector<float> v_time;
   std::vector<float> v_voltage;
   float noise_rms;
   // std::fstream outputFile;
   /// TREE for charge
   TFile *f = new TFile(Form("%s",argv[8]),"RECREATE");
   TTree* T = new TTree("T","Main tree for data and results");
   T->Branch("index",&i2,"index/I");
   T->Branch("charge",&carga,"charge/F");

   //TTree* TrawPulses = new TTree("TrawPulses","Raw Pulses");
   T->Branch("time",&v_time);
   T->Branch("voltage",&v_voltage);
   T->Branch("noise",&noise_rms);
   // ROOT file name
   std::cout << outRootFile << std::endl;

   //Nombre de mi archivo de salida
 //  outputFile.open(outDatFile,std::ios::out | std::ios::trunc);
// Estimando la carga mediante una sumatoria
  do
  {
   tiempo=1;
   Carga_actual=0;
   Carga_anterior=0;
   carga=0;
   voltaje_max=-1000;
   voltaje_min=1000; 
   sum_volt=0;
   sum_ruido1=0;
   sum_ruido2=0;
   avg_ruido1=0;
   avg_ruido2=0;
   volt=0;
   for( i=0; i<No_muestras; i++ )
   {

     fscanf( fichero, "%f,%f\n", &x1, &volt);
     v_time.push_back(x1);
     v_voltage.push_back(volt);
     if (i<bin_inicial-1)
     {
       v_noise.push_back(volt);
       sum_ruido1+=volt;

     }
     if (i>=bin_inicial-1 && i<(bin_inicial+ancho)-1)
     {
       if (voltaje_max<volt)
       {
         voltaje_max=volt;
       }
       if (voltaje_min>volt)
       {
         voltaje_min=volt;
       }
       sum_volt+=volt;
     }
     if (i>=(bin_inicial + ancho)-1)
     {
       sum_ruido2+=volt;
     }
   }

  avg_ruido1= (1/amplifGain) *(sum_ruido1)/(bin_inicial-1);
  avg_ruido2=(1/amplifGain) *(sum_ruido2)/(No_muestras-bin_inicial-ancho+1);
  sum_volt=sum_volt*(1/amplifGain);
  
  ct10=carga*0.1;
  ct50=carga*0.5;
  ct90=carga*0.9;
  prom_max=prom_max+voltaje_max;
  prom_min=prom_min+voltaje_min;
  if (bin_inicial==1)
  {
     carga=sum_volt;
     voltaje_max=voltaje_max;
     voltaje_min=voltaje_min;
  }
  else
  {
     //carga= sum_volt-(ancho*avg_ruido1);
    carga= flipConstant*(delta_t*(sum_volt-(avg_ruido1*ancho)))/(R*picoScale);
     voltaje_max=voltaje_max-avg_ruido1;  //printf( "....................%u\n", i2);

     voltaje_min=voltaje_min-avg_ruido1;
  }

// rms
  
noise_rms= 6*rms(v_noise);


//   for( iprima=it; iprima<it+No_muestras; iprima++ )
//   {
//      fscanf( fichero2,  "%f\t%f\n", &x1, &x2);
//      if (iprima>=it+bin_inicial-1 && iprima<it+(bin_inicial+ancho)-1)
//      {
//         Carga_actual=Carga_actual+x2;
//         if (Carga_actual >= ct10 && Carga_anterior < ct10)
//         {
//            t10=(ct10-Carga_anterior)/(Carga_actual-Carga_anterior)+(tiempo-1);
//         }
//         if (Carga_actual >= ct50 && Carga_anterior < ct50)
//         {
//            t50=(ct50-Carga_anterior)/(Carga_actual-Carga_anterior)+(tiempo-1);
// }
//         if (Carga_actual >= ct90 && Carga_anterior < ct90)
//         {
//            t90=(ct90-Carga_anterior)/(Carga_actual-Carga_anterior)+(tiempo-1);
//         }
//         Carga_anterior=Carga_actual;
//         tiempo=tiempo+1;
//      }
//      }
  T->Fill();
  v_time.clear();
  v_voltage.clear();
  v_noise.clear();
  
 // outputFile <<i2<<"	"<<carga<<std::endl;//"	"<<voltaje_min<<"	"<<voltaje_max<<"	"<<t50-t10<<"	"<<t90-t10<<std::endl;
  //  std::cout << it << '\n' ;

  i2=i2+1;
  
//  std::cout << it << '\n' ;
  

  }
  while (i2<=No_eventos);

  prom_max=prom_max/No_eventos;
  prom_min=prom_min/No_eventos;
  //outputFile.close();
  T->Write();
 // f->Write();
  f->Close();
  //if( !fclose(fichero) && !fclose(fichero2) )
  //   printf( "Fichero cerrado\n" );
  //else
 // {
   //  printf( "Error: fichero NO CERRADO\n" );
   //  return 1;
 // }

  return 0;
}

