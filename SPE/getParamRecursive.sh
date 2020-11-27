#! /bin/bash

# Generate plots and result files from pre-processed text files
# that were generated by merge_and_processes.sh script.

# The input is the directory containing dat files with the charge
# value for each event.

# The output are the plots and a file with the extracted SPE parameters
# using the getSPEparam.C root macro.

execPath=/home/salvador/github/proyectitosWChava/SPE
inputPath=/home/salvador/github/proyectitosWChava/SPE/data/processed/
outputFileName=SPEparams
rm /home/salvador/github/proyectitosWChava/SPE/data/SPEparam/SPEparams.dat
cd $inputPath
for file in *
do
mergePath="${inputPath}""${file}"
echo $mergePath
cd $execPath
./getSPEparam $mergePath $outputFileName
cd $inputPath
done
cd /home/salvador/github/proyectitosWChava/SPE/data/plots/
rm montage.png
montage -geometry +2+2 *.png montage.png