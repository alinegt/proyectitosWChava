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
#path=/home/salvador/Documents/WPT/data/coils_X0_Y0_2nd
dataPath=/home/salvador/Documents/WPT/data_dec_03/
#dataPath=/home/salvador/Documents/WPT/data 
mergePath=/home/salvador/github/proyectitosWChava/SPE/data/merge/
processedPath=/home/salvador/github/proyectitosWChava/SPE/data/processed/
scriptsPath=//home/salvador/github/proyectitosWChava/SPE/

# dirName="${path##*/}"
# mergeName="${dirName}""_merge.dat"
# processedName="${dirName}"".dat"

cd $dataPath
for dir in */
do

dirName="${dir%/}"
mergeName="${dirName}""_merge.dat"
processedName="${dirName}"".dat"

echo $dirName
echo $mergeName
cd $dir
# rm $mergeName

rm *.dat

for file in *
do
nameOnly="${file##*/}"
nameOnly=${file%.*}
dataFileName="$nameOnly.dat"

tail -n 200 $file >> $dataFileName

#echo $file $nameOnly $dataFileName
done 

cat *.dat > $mergePath$mergeName

cd $scriptsPath

./processWaveforms $mergePath$mergeName 100 60 200 19980 5e-10 $processedPath$processedName
#root plotHistFromFile.C
# echo $dirName
cd $dataPath
done

exit

