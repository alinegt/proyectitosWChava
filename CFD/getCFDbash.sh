#! /bin/bash
make
execPath=/home/salvador/github/proyectitosWChava/CFD
inputPath=/home/salvador/github/proyectitosWChava/SPE/data/processed/
outputPath=/home/salvador/github/proyectitosWChava/CFD/data/rootFiles/
fileExt=".root"

make
cd $inputPath

for file in *
do
#inputPath=/home/salvador/github/proyectitosWChava/SPE/data/processed/
outputPath=/home/salvador/github/proyectitosWChava/CFD/data/rootFiles/

processedPath="${inputPath}""${file}"
nameOnly="${file##*/}"
nameOnly="${file%.*}"
outputPath="${outputPath}""${nameOnly}""${fileExt}"
echo $processedPath
echo $outputPath
cd $execPath
./getTimeCFD $processedPath $outputPath 
cd $inputPath
#processedPath=""
#outputPath=""
done
#cd ${outputPath}

# rm SPEmontage.png
# montage -geometry 1000x1000+2+2  *.png SPEmontage.png
