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

#path=/home/salvador/Documents/WPT/data/noCoil
# path=/home/salvador/Documents/WPT/data/noCoil2
# path=/home/salvador/Documents/WPT/data/coils_X0_Y0_2nd
 path=/home/salvador/Documents/WPT/data/coils_X0_Y0_box



dirName="${path##*/}"
mergeName="${dirName}""_merge.dat"
processedName="${dirName}""_charge.dat"

echo $dirName
cd $path
rm $mergeName

rm *.dat

for file in *
do
nameOnly="${file##*/}"
nameOnly=${file%.*}
dataFileName="$nameOnly.dat"

tail -n 200 $file >> $dataFileName

#echo $file $nameOnly $dataFileName
done 

cat *.dat > /home/salvador/github/proyectitosWChava/$mergeName

cd /home/salvador/github/proyectitosWChava/

./processWaveforms $mergeName 80 60 200 10989 5e-10 $processedName
#root plotHistFromFile.C
echo $dirName
exit

