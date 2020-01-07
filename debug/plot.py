import numpy as np
from matplotlib import  pyplot as plt
data=np.fromfile("/home/gnu/Desktop/BleRffSniffer/Binary","int16")[:1000000]
data=data.reshape(-1,2)/2048.0
data=data[:,0]+data[:,1]*1j
# plt.plot(np.abs(data))
# plt.scatter(x=data.real,y=data.imag,s=1)
plt.plot(data.real,data.imag)
plt.show()