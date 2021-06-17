#! /bin/bash

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

runName=$1
execPath=/home/idlab-52/github/proyectitosWChava/timing
inputPath=/home/idlab-52/github/proyectitosWChava/SPE/data/cutRootFiles/$runName/
outputPath=/home/idlab-52/github/proyectitosWChava/timing/data/rootFiles/$runName/
plotsPath=/home/idlab-52/github/proyectitosWChava/timing/data/plots/$runName/

fileExt=".root"
make -B
# cd "$execPath""/data""/plots/"

createDir $plotsPath
createDir $outputPath

cd $inputPath

for file in *
do
#inputPath=/home/idlab-52/github/proyectitosWChava/SPE/data/processed/
# outputPath=/home/idlab-52/github/proyectitosWChava/timing/data/rootFiles/

processedPath="${inputPath}""${file}"
nameOnly="${file##*/}"
nameOnly="${file%.*}"
outputPath="${outputPath}""${nameOnly}""${fileExt}"
echo $processedPath
echo $outputPath
cd $execPath
./getTimeCFD  $processedPath $outputPath  $1
cd $inputPath
processedPath=""
outputPath=""
done
cd $plotsPath
montage -geometry +2+2  *.png "$1""montage.png"

# cd $execPath
#root -l 'printRootContent.C("./data/rootFiles/mergeRoot.root")'                                                                                                      ─╯
# root -l plotStackLE2.C
# root -l plotStackCFD2.C

# root -l plotStack.C
# root -l plotStackLE.C
#cd ${outputPath}

# rm SPEmontage.png
