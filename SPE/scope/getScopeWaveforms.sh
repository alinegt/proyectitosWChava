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

#gawk '{a[NR]=$1}END{for(i=1;i<=$nbrWaveforms;i++)for(j=1;j<=NR;j++)print a[j]}' $filename$time$csv >  $filename$time$compTime$csv 

awk -v n=$nbrWaveforms '{s = s $1 ORS} END{for (i=1;i<=n;i++) printf "%s", s}' $filename$time$csv >  $filename$time$compTime$csv 

paste $filename$time$compTime$csv   $filename$csv > ../dataOutput/$filename$out$dat


cd ../
echo $PWD
python3 saveFigScope.py $filename$out

sed -i 's/\t/,/g' ./dataOutput/$filename$out$dat                                   

#sed 's/,/ /g' ./dataScope/$filename$csv > $filename$dat                                   
#for i in {2..$nbrWaveforms}
#do
#    awk'{print $i}' $filename$dat> voltage.dat
#done
#awk 'BEGIN{ ORS="" } { for ( i=2; i<= NF ; i++){ dict[i]=dict[i]$i"\n"  }  } END { for (key in dict) { print dict[key] }  }'  $filename$dat> voltage.dat
#awk 'BEGIN{ ORS="" } { for ( i=2; i<= NF ; i++){ dict[i]=dict[i]$1"\n"  }  } END { for (key in dict) { print dict[key] }  }'  $filename$dat> time.dat 

#awk 'BEGIN{ ORS="" } { for ( i=2; i<= NF ; i++){ print $i"\n"  }  }' $filename$dat> voltage.dat 
#awk 'BEGIN{ ORS="" } { for ( i=2; i<= NF ; i++){ print $1"\n"  }  }' $filename$dat> time.dat 
#awk '{for(i=2;i<=NF;i++){ print $i}}' $filename$dat> voltage.dat 
#awk '{for(i=2;i<=NF;i++){ print $1}}' $filename$dat> time.dat 


#paste time.dat voltage.dat > $filename$stacked$dat




#rm voltage.dat
#rm time.dat
#rm $filename$dat
#rm  $filename$stacked$dat
#python3 stackColumns.py  $filename $nbrWaveforms
end_=$(date)
echo $end_


#echo "saving image"

