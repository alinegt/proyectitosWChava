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
rm processWaveforms
cd ./data/3650mV_17ns
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
cat *.dat > ../../merge.dat
cd ../../
g++ processWaveforms.cxx -o processWaveforms
./processWaveforms
root plotHistFromFile.C

exit

