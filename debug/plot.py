import numpy as np
from matplotlib import  pyplot as plt
for i in range(1,1000):
    plt.figure(figsize=(6, 6))
    plt.rcParams['agg.path.chunksize'] = 10000
    data=np.fromfile("/home/zhanglei/BleFpSniffer/build/data/"+str(i),"int16")
    data=data.reshape(-1,2)/2048.0
    data=data[:,0]+data[:,1]*1j
    # plt.plot(np.abs(data))
    plt.scatter(x=data.real,y=data.imag,s=1)
    # plt.plot(data.real,data.imag)
    plt.savefig("/home/zhanglei/BleFpSniffer/build/pic/"+str(i))
    plt.close()