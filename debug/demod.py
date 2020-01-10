import numpy as np
data=np.fromfile("/home/gnu/Desktop/BleRffSniffer/build/data/2","int16")
data=data.reshape(-1,2)
data=data[:,0]+data[:,1]*1j
data=data[:-1].real*data[1:].imag-data[:-1].imag*data[1:].real
# data=data[1:]*data[:-1].conjugate()
# data=np.arctan2(data.imag,data.real)*(20/3.1415926)
bit=[]
count=0
for i in range(0,len(data),10):
    # print(int(i),end="")
    bit.append(1) if data[i ]>0 else bit.append(0)
for i in range(0,len(bit)):
    if (bit[i:i+40]==[0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,1,1,1,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,1]):
        count+=1
print(count)