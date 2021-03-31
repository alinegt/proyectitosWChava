#!/usr/bin/env python
# coding: utf-8
import sys
import pandas as pd
import matplotlib.pyplot as plt

filename=sys.argv[1]
plt.rcParams['figure.figsize'] = [12, 8]
plt.rcParams['figure.dpi'] = 100 # 200 e.g. is really fine, but slower
plt.rcParams.update({'font.size': 18})

nmbrOfWaveforms= int(sys.argv[2])
columns=['time']
for i in range(0,nmbrOfWaveforms):
    columns.append('waveform_{}'.format(i))
df=pd.read_csv("./dataScope/"+filename+".csv",names=columns)
fig=df.plot(legend=False).get_figure()
fig.savefig(filename+'.png')


dfVoltage=df.drop(['time'], axis=1)
dfVoltage1col=pd.Series(dfVoltage.values.ravel('F'))
dfVoltage1col=dfVoltage1col.rename('Voltage')
time_list=df['time'].values.tolist()*10
dftime=pd.DataFrame(time_list, columns=['time'])
dfOutput=pd.concat([dftime, dfVoltage1col], axis=1)
dfOutput.to_csv('./dataOutput/'+filename+'.dat', index=False,header=False)



