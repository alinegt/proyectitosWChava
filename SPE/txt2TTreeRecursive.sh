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
merge_datSufix="_merge.dat"
dat=".dat"
out_RootSufix=".root"

# Deleting last run files
rm $mergePath*.dat
rm $processedPath*.root
rm $cuttedPath*.root
make
cd $dataPath


dirName="${dir%/}"

echo $dirName
echo $mergePath
#cd $dir


approx_pulse_width=60
number_of_points=200
number_of_waveforms=100000
delta_time=5e-10

for file in *
do
echo "fileName"
echo $file
# cp $file $mergePath

nameOnly="${file##*/}"
nameOnly=${file%.*}
mergeName=$nameOnly
processedName=$nameOnly$dat
outRootFile=$nameOnly$out_RootSufix
echo $mergeName

echo $dataPath$mergeName$processedSufix


cd $scriptsPath

./txt2TTree $dataPath$file $noiseMaxIndex $approx_pulse_width $number_of_points $number_of_waveforms 5e-10 $processedPath$processedName $processedPath$outRootFile 1

# cd $dataPath
done

exit

