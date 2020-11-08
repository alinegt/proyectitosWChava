
//Este programa procesa los datos tomados por el sistema de adquisición de datos de CAEN, y se obtienen un archivo con datos de salida

#include <iostream>
#include <fstream> 
#include <stdio.h>

int main(void)
{
   FILE *fichero;
   FILE *fichero2;
// char nombre[10] = "datos.dat";
   unsigned int i,iprima,i2=1; 
   float x1=0, x2=0, x3=0, ch2=0, ch3, ch4;
   float carga=0, it=1;
   int voltaje_max=0, voltaje_min=0;
   float prom_max=0, prom_min=0;
   float ruido1=0, ruido2=0;
   float t10=0, t50=0, t90=0, ct10=0, ct50=0, ct90=0;
   float Carga_actual=0, Carga_anterior=0, tiempo=1;
   char c1[10],c2[10],c3[10],c4[10],c5[10];
   //Nombre de mi archivo de entrada
   fichero = fopen( "merge.dat", "r" );
   fichero2 = fopen( "merge.dat", "r" );

/************************************************************************************************/
/****************************************VARIABLES MENU******************************************/
   unsigned int bin_inicial=100; //Dónde comienza a hacer el proceso
   unsigned int ancho=25;        //Ancho del pulso aproximado
   float No_muestras=200;        //Dato puesto durante la adquisición, en nanosegundos
   float No_eventos=999;       //Dato puesto durante la adquisición, cantidad de eventos tomados

/************************************************************************************************/

   std::fstream outputFile;
   //Nombre de mi archivo de salida
   outputFile.open("processedWaveforms.dat",std::ios::out | std::ios::trunc);
// std::cout<<var_sig<<std::endl;
// Estimando la carga mediante una sumatoria
  do
  {
   tiempo=1;
   Carga_actual=0;
   Carga_anterior=0;
   carga=0;
   ruido1=0;
   ruido2=0;
   voltaje_max=-1000;
   voltaje_min=1000; 
   for( i=it; i<it+No_muestras; i++ )
   {
     fscanf( fichero, "%f,%f\n", &x1, &x2);
     if (i<it+bin_inicial-1)
     {
       ruido1=ruido1+x2;
     }
     if (i>=it+bin_inicial-1 && i<it+(bin_inicial+ancho)-1)
     {
       if (voltaje_max<x2)
       {
         voltaje_max=x2;
       }
       if (voltaje_min>x2)
       {
         voltaje_min=x2;
       }
       carga=carga+(x2);
     }
     if (i>=it+(bin_inicial + ancho)-1)
     {
       ruido2=ruido2+x2;
     }
   }
//  std::cout<<ruido1<<"     "<<ruido2<<std::endl;
  ruido1=ruido1/(bin_inicial-1);
  ruido2=ruido2/(No_muestras-bin_inicial-ancho+1);
//  std::cout<<ruido1<<"     "<<ruido2<<std::endl;
//  if (ruido1<=ruido2)
//  {
//    ruido1=ruido2;
//  }
  ct10=carga*0.1;
  ct50=carga*0.5;
  ct90=carga*0.9;
  prom_max=prom_max+voltaje_max;
  prom_min=prom_min+voltaje_min;
  if (bin_inicial==1)
  {
     carga=carga;
     voltaje_max=voltaje_max;
     voltaje_min=voltaje_min;
  }
  else
  {
     carga=carga-(ancho*ruido1);
     voltaje_max=voltaje_max-ruido1;
     voltaje_min=voltaje_min-ruido1;
  }

  for( iprima=it; iprima<it+No_muestras; iprima++ )
  {
     fscanf( fichero2,  "%f\t%f\n", &x1, &x2);
     if (iprima>=it+bin_inicial-1 && iprima<it+(bin_inicial+ancho)-1)
     {
        Carga_actual=Carga_actual+x2;
        //std::cout<<Carga_actual<<"     "<<Carga_anterior<<"    "<<ct10<<"    "<<ct50<<"    "<<ct90<<std::endl;
        if (Carga_actual >= ct10 && Carga_anterior < ct10)
        {
           t10=(ct10-Carga_anterior)/(Carga_actual-Carga_anterior)+(tiempo-1);
        //std::cout<<t10<<"...................................q......t10"<<std::endl;
        }
        if (Carga_actual >= ct50 && Carga_anterior < ct50)
        {
           t50=(ct50-Carga_anterior)/(Carga_actual-Carga_anterior)+(tiempo-1);
//std::cout<<t50<<".........................................t50"<<std::endl;        
}
        if (Carga_actual >= ct90 && Carga_anterior < ct90)
        {
           t90=(ct90-Carga_anterior)/(Carga_actual-Carga_anterior)+(tiempo-1);
//std::cout<<t90<<".........................................t90"<<std::endl;
        }
        Carga_anterior=Carga_actual;
        tiempo=tiempo+1;
     }
  }
  outputFile <<i2<<"	"<<carga/50<<"	"<<voltaje_min<<"	"<<voltaje_max<<"	"<<t50-t10<<"	"<<t90-t10<<std::endl;
  printf( "....................%u\n", i2);
  i2=i2+1;
  it=x1;
  }
  while (i2<=No_eventos);
  prom_max=prom_max/No_eventos;
  prom_min=prom_min/No_eventos;
  outputFile.close();
  //std::cout<<ruido1;
  if( !fclose(fichero) && !fclose(fichero2) )
     printf( "Fichero cerrado\n" );
  else
  {
     printf( "Error: fichero NO CERRADO\n" );
     return 1;
  }

  return 0;
}

