#! /bin/bash
filename=$1    #2800mVpp_506mV_110ns_82_ns  
nbrWaveforms=$2
csv=".csv"
dat=".dat"
out="_out"
time="_time"
compTime="_xtimes"


start_=$(date)
echo $start_
python3 Get_InfiniiVisionX_Waveforms.py $filename $nbrWaveforms
end_=$(date)
echo $end_

cd dataScope


awk -v n=$nbrWaveforms '{s = s $1 ORS} END{for (i=1;i<=n;i++) printf "%s", s}' $filename$time$csv >  $filename$time$compTime$csv 

paste $filename$time$compTime$csv   $filename$csv > ../dataOutput/$filename$out$dat

rm $filename$time$compTime$csv 

cd ../
echo $PWD
python3 saveFigScope.py $filename$out

datFileName=$filename$out$dat
sed -i 's/\t/,/g' ./dataOutput/$datFileName                                   

cd dataOutput

dataPath=$PWD/
echo $dataPath$datFileName
noiseMaxIndex=160
approx_pulse_width=128
numberOfLinesTotal=$(wc -l < $datFileName)
echo $numberOfLinesTotal
let numberOfpoints=$numberOfLinesTotal/$nbrWaveforms
#Get delta time
first_number=$(awk -F',' -M 'NR==50 {printf "%.16f", $1+0; exit}' $datFileName)
second_number=$(awk -F',' -M 'NR==51 {printf "%.16f", $1+0; exit}' $datFileName)
echo $first_number
echo $second_number
delta_time=$(awk '{print $1-$2}' <<< "$second_number $first_number")
echo $delta_time

out_RootSufix=".root"
outRootFile=$filename$out_RootSufix
processedPath=$HOME/github/proyectitosWChava/SPE/scope/rootFiles/
echo $processedPath$outRootFile
echo txt2ttree
txt2TTree $dataPath$datFileName $noiseMaxIndex $approx_pulse_width $numberOfpoints $nbrWaveforms $delta_time $processedPath$outRootFile

syncRootFiles 

end_=$(date)
echo $end_


#echo "saving image"

