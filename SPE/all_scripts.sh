#! /bin/bash

nofWaveforms=$1
noiseMaxIndex=$2
name=$3
cd /home/idlab-52/github/proyectitosWChava/SPE/

timePlotsDir=$PWD/data/timePlots/$name
spePlotsDir=$PWD/data/SPEplots/$name
NoiseplotsDir=$PWD/data/NoisePlots/$name
outputDir=$PWD/data/output/$name
cutRootFilesDir=$PWD/data/cutRootFiles/$name

echo $localPath
# ./txt2TTreeRecursive.sh /home/idlab-52/github/proyectitosWChava/SPE/scope/dataOutput/optFiber/2e5_2/ $nofWaveforms  $noiseMaxIndex   #data_200ns/second $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise

function createDir(){

if [[ -d "$1" ]]; then
    echo "$1 exists on your filesystem."
    rm $1/*
else 
    echo "$1 does not exist on your filesystem."
    echo Creating $1 directory
    mkdir $1
fi

} 


createDir $timePlotsDir
createDir $spePlotsDir
createDir $NoiseplotsDir
createDir $outputDir
createDir $cutRootFilesDir


getParamRecursive.sh $name $noiseMaxIndex "noOutliers==1"



# name2=$3

# ./merge_and_process.sh  /home/salvador/Documents/WPT/data_200ns/$name2 $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
# ./getParamRecursive.sh $name2 $noiseMaxIndex 


# ./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/alumBox
# ./getParamRecursive.sh
