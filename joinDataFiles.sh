#! /bin/bash
#This file transforms all the data files from the folder (scope data) into one single data file.

cd /home/aline/Documents/DataAnalysis/procrastinando_conChava/SPE

for file in *
do
nameOnly="${file##*/}"
nameOnly=${file%.*}
dataFileName="$nameOnly.dat"

tail -n 200 $file >> $dataFileName

#echo $file $nameOnly $dataFileName
done 
cat *.dat > merge.dat

exit
