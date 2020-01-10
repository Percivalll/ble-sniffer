import numpy as np
from matplotlib import  pyplot as plt
plt.figure(figsize=(6, 6))
data=np.fromfile("/home/gnu/Desktop/BleRffSniffer/Binary","int16")[80000:90000]
data.tofile("/home/gnu/Desktop/BleRffSniffer/Asample")
# data=data.reshape(-1,2)/2048.0
# data=data[:,0]+data[:,1]*1j
# plt.plot(np.abs(data))
# plt.scatter(x=data.real,y=data.imag,s=1)
# plt.plot(data.real,data.imag)
# plt.show()