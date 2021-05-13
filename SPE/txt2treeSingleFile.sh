
#! /bin/bash
datFileName=$1

nameOnly="${datFileName##*/}"
nameOnly=${datFileName%.*}

nbrWaveforms=$2
dataPath=$PWD/
echo $dataPath$datFileName
noiseMaxIndex=160
approx_pulse_width=128
numberOfLinesTotal=$(wc -l < $datFileName)
echo $numberOfLinesTotal
let numberOfpoints=$numberOfLinesTotal/$nbrWaveforms
#Get delta time
first_number=$(awk -F',' -M 'NR==50 {printf "%.16f", $1+0; exit}' $datFileName)
second_number=$(awk -F',' -M 'NR==51 {printf "%.16f", $1+0; exit}' $datFileName)
echo $first_number
echo $second_number
delta_time=$(awk '{print $1-$2}' <<< "$second_number $first_number")
echo $delta_time

out_RootSufix=".root"
outRootFile=$nameOnly$out_RootSufix
processedPath=$HOME/github/proyectitosWChava/SPE/scope/rootFiles/
echo $processedPath$outRootFile
echo txt2ttree
txt2TTree $dataPath$datFileName $noiseMaxIndex $approx_pulse_width $numberOfpoints $nbrWaveforms $delta_time $processedPath$outRootFile

syncRootFiles 
