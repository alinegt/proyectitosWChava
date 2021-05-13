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
noiseMaxIndex=$3
localPath="/home/idlab-52/github/proyectitosWChava/SPE"
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


approx_pulse_width=128  #290
number_of_waveforms=$2
# delta_time=4.688e-10  #6.25e-10

for file in *
do
echo "fileName"
echo $file
echo $PWD
# cp $file $mergePath
# Getting number of points in waveform 
numberOfLinesTotal=$(wc -l < $file)
echo $numberOfLinesTotal
let numberOfpoints=$numberOfLinesTotal/$number_of_waveforms
echo $numberOfpoints
#Get delta time
first_number=$(awk -F',' -M 'NR==50 {printf "%.16f", $1+0; exit}' $file)
second_number=$(awk -F',' -M 'NR==51 {printf "%.16f", $1+0; exit}' $file)
echo $first_number
echo $second_number
delta_time=$(awk '{print $1-$2}' <<< "$second_number $first_number")
echo $delta_time
# Names
nameOnly="${file##*/}"
nameOnly=${file%.*}
mergeName=$nameOnly
processedName=$nameOnly$dat
outRootFile=$nameOnly$out_RootSufix
echo $mergeName

# echo $dataPath$mergeName$processedSufix


cd $scriptsPath

./txt2TTree $dataPath$file $noiseMaxIndex $approx_pulse_width $numberOfpoints $number_of_waveforms $delta_time $processedPath$processedName $processedPath$outRootFile

cd $dataPath
done

exit

