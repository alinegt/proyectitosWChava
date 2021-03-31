#! /bin/bash
name=$1
noiseMaxIndex=$2

./txt2TTree.sh  /home/salvador/Documents/WPT/data_150/ $noiseMaxIndex   #data_200ns/second $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
./getParamRecursive.sh $name $noiseMaxIndex 


# name2=$3

# ./merge_and_process.sh  /home/salvador/Documents/WPT/data_200ns/$name2 $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
# ./getParamRecursive.sh $name2 $noiseMaxIndex 


# ./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/alumBox
# ./getParamRecursive.sh
