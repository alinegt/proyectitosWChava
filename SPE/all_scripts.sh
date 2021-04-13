#! /bin/bash
nofWaveforms=$1
noiseMaxIndex=$2
name=$3


./txt2TTreeRecursive.sh /home/idlab-52/github/proyectitosWChava/SPE/scope/dataOutput/coilTest2/ $nofWaveforms  $noiseMaxIndex   #data_200ns/second $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
./getParamRecursive.sh $name $noiseMaxIndex "noOutliers==1"


# name2=$3

# ./merge_and_process.sh  /home/salvador/Documents/WPT/data_200ns/$name2 $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
# ./getParamRecursive.sh $name2 $noiseMaxIndex 


# ./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/alumBox
# ./getParamRecursive.sh
