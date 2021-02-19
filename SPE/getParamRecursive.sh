#! /bin/bash

# Generate plots and result files from pre-processed text files
# that were generated by merge_and_processes.sh script.

# The input is the directory containing dat files with the charge
# value for each event.

# The output are the plots and a file with the extracted SPE parameters
# using the getSPEparam.C root macro.
make -B
localPath=$PWD
execPath=$localPath
inputPath=$localPath/data/processed/
outputFileName=SPEparams
mergePath=$localPath/data/merge/

rm $localPath/data/occupancy/*.png
rm $localPath/data/plots/*.png
rm $localPath/data/timePlots/*.png
rm $localPath/data/SPEparam/SPEparams.dat

cd $inputPath
for file in *
do
processedPath="${inputPath}""${file}"
echo $mergeFile
echo $processedPath
cd $execPath
./SPE_and_timing $processedPath $outputFileName 
cd $inputPath
done

cd $localPath/data/plots/
montage -geometry 1000x1000+2+2  *.png SPEmontage.png

cd $localPath/data/timePlots/
montage -geometry 1000x1000+2+2 *.png Timemontage.png