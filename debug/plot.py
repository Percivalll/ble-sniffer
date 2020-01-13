import numpy as np
from matplotlib import  pyplot as plt
plt.figure(figsize=(6, 6))
# data=np.fromfile("/home/gnu/Desktop/BleRffSniffer/build/data/45","int16")
data=np.fromfile("/home/gnu/Desktop/BleRffSniffer/Binary","int16")
# data=data.reshape(-1,2)
# data=data[:,0]+data[:,1]*1j
# plt.plot(np.abs(data))
# # plt.scatter(x=data.real,y=data.imag,s=1)
# # plt.plot(data.real,data.imag)
# plt.show()
