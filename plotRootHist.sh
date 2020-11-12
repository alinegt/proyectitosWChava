#! /bin/bash
#This script 
# 1) Merge all the data files from the folder (scope data) into one single data file
# 2) Generates executable for Charge estimation
# 3) Plot a Histogram on ROOT

# Dependencies:
# processWaveforms.cxx, plotHistFromFile.C

# Outputs:
# merge.dat
# processedWaveforms.dat  (charge data)
# processWaveforms  (executable)
rm merge.dat

cd /home/salvador/Documents/WPT/data/3650mV_17ns_2_coils_2nd_level
#cd /home/salvador/Documents/WPT/data/3650mV_17ns_2
#cd ./data/SPE
rm *.dat

for file in *
do
nameOnly="${file##*/}"
nameOnly=${file%.*}
dataFileName="$nameOnly.dat"

tail -n 200 $file >> $dataFileName

#echo $file $nameOnly $dataFileName
done 
cat *.dat > /home/salvador/github/proyectitosWChava/merge.dat

cd /home/salvador/github/proyectitosWChava/

./processWaveforms merge.dat 90 40 200 10989 5e-10 processedWaveforms_coil.dat
#root plotHistFromFile.C

exit

