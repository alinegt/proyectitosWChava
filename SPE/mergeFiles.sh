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

dataPath=$1
noiseMaxIndex=$2
localPath=$PWD
mergePath=$localPath/data/merge/
processedPath=$localPath/data/processed/
scriptsPath=$localPath/
cuttedPath=$localPath/data/cutted/

# Deleting last run files
rm $mergePath*.dat
rm $processedPath*.root
rm $cuttedPath*.root
make
cd $dataPath
for dir in */
do
dirName="${dir%/}"
mergeName="${dirName}""_merge.dat"
processedName="${dirName}"".dat"
outRootFile="${dirName}"".root"
echo $dirName
echo $mergeName
cd $dir

rm *.dat

approx_pulse_width=60
number_of_points=200
number_of_waveforms=19980
delta_time=5e-10

for file in *
do
nameOnly="${file##*/}"
nameOnly=${file%.*}
dataFileName="$nameOnly.dat"

tail -n 200 $file >> $dataFileName

done 
 
cat *.dat > $mergePath$mergeName


cd $dataPath
done

exit

