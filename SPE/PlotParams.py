# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams['figure.figsize'] = [12, 8]
plt.rcParams['figure.dpi'] = 100 # 200 e.g. is really fine, but slower
plt.rcParams.update({'font.size': 18})
# %%
df=pd.read_csv("./data/SPEparam/alumBox.dat", header=None, sep=" ", usecols=[0,1],names=["id","Vrms"])

dist=[10,20,30,40,50]
df.insert(loc=0, column="Distance",value=dist)
df=df.set_index("Distance")


df


# %%
df2=pd.read_csv("./data/SPEparam/shielding.dat", header=None, sep=" ", usecols=[0,1], names=["id","Vrms"])
df2.insert(loc=0, column="Distance",value=dist)
df2=df2.set_index("Distance")

df2


# %%
ax=df["Vrms"].plot(marker='o', label='Aluminum Box')
df2["Vrms"].plot(marker="s", label="Mu metal Shielding", ax=ax)
ax.legend()
plt.ylabel(ylabel='Amplitude [Vrms]')
plt.xlabel(xlabel='Distance[cm]')
plt.show()
# df2.transpose().plot(marker='o',ax=ax)
# fonttam=17


# plt.xticks(fontsize=fonttam)
# plt.yticks(fontsize=fonttam)
# plt.xticks(rotation=75)


# %%



# %%



