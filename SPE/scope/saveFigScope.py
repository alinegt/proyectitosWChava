#!/usr/bin/env python
# coding: utf-8
import sys
import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams['agg.path.chunksize'] = 10000

filename=sys.argv[1]
plt.rcParams['figure.figsize'] = [12, 8]
plt.rcParams['figure.dpi'] = 100 # 200 e.g. is really fine, but slower
plt.rcParams.update({'font.size': 18})
columns=['time','voltage']
#nmbrOfWaveforms= int(sys.argv[2])
df=pd.read_csv("./dataOutput/"+filename+".dat", header=None, delimiter="\t", names=columns)
print(df)
fig=df.plot(x='time',y='voltage',legend=False).get_figure()
fig.savefig("./images/"+filename+'.png')



