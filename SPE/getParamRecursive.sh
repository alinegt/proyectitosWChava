#! /bin/bash

# Generate plots and result files from pre-processed text files
# that were generated by merge_and_processes.sh script.

# The input is the directory containing dat files with the charge
# value for each event.

# The output are the plots and a file with the extracted SPE parameters
# using the getSPEparam.C root macro.
make
execPath=/home/salvador/github/proyectitosWChava/SPE
inputPath=/home/salvador/github/proyectitosWChava/SPE/data/processed/
outputFileName=SPEparams
mergePath=/home/salvador/github/proyectitosWChava/SPE/data/merge/

rm /home/salvador/github/proyectitosWChava/SPE/data/SPEparam/SPEparams.dat
cd $inputPath
for file in *
do
processedPath="${inputPath}""${file}"
# mergeFile="${mergePath}""${file%.*}""_merge.dat"
echo $mergeFile
echo $processedPath
cd $execPath
./getSPEparam $processedPath $outputFileName 
cd $inputPath
done
cd /home/salvador/github/proyectitosWChava/SPE/data/plots/
rm SPEmontage.png
montage -geometry 1000x1000+2+2  *.png SPEmontage.png

cd /home/salvador/github/proyectitosWChava/SPE/data/timePlots/
rm Timemontage.png
montage -geometry 1000x1000+2+2 *.png Timemontage.png