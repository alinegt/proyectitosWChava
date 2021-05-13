#! /bin/bash
runName=$1
execPath=/home/idlab-52/github/proyectitosWChava/timing
inputPath=/home/idlab-52/github/proyectitosWChava/SPE/data/cutRootFiles/$runName/
outputPath=/home/idlab-52/github/proyectitosWChava/timing/data/rootFiles/
fileExt=".root"
make -B
cd $inputPath

for file in *
do
#inputPath=/home/idlab-52/github/proyectitosWChava/SPE/data/processed/
outputPath=/home/idlab-52/github/proyectitosWChava/timing/data/rootFiles/

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

montage -geometry 1000x1000+2+2  *.png SPEmontage.png

# cd $execPath
#root -l 'printRootContent.C("./data/rootFiles/mergeRoot.root")'                                                                                                      ─╯
# root -l plotStackLE2.C
# root -l plotStackCFD2.C

# root -l plotStack.C
# root -l plotStackLE.C
#cd ${outputPath}

# rm SPEmontage.png
