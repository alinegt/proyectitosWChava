#! /bin/bash
noiseMaxIndex=$1
./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/shielding $noiseMaxIndex  #/home/salvador/Documents/WPT/data_feb8_2021/toPresent/Nocoil  # /home/salvador/Documents/WPT/noise
./getParamRecursive.sh shielding $noiseMaxIndex 
# ./merge_and_process.sh  /home/salvador/Documents/WPT/Zscan_2021/alumBox
# ./getParamRecursive.sh