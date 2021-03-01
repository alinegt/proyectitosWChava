#! /bin/bash
name=$1
noiseMaxIndex=$2

./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/$name $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
./getParamRecursive.sh $name $noiseMaxIndex 


name2=$3

./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/$name2 $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
./getParamRecursive.sh $name2 $noiseMaxIndex 


# ./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/alumBox
# ./getParamRecursive.sh