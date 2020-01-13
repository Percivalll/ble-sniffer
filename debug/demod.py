import numpy as np
data=np.fromfile("/home/gnu/Desktop/BleRffSniffer/build/data/4","int16")
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
byte=[]
i=0
while(i<len(bit)):
    temp=0
    for j in range(8):
        temp=temp|(bit[i]<<j)
        i+=1
    byte.append(temp)
byte=byte[5:47]
for i in byte:
    print('0x%x'%i,end=" ")
print()
table=[141, 210, 87, 161, 61, 167, 102, 176, 117, 49, 17, 72, 150, 119, 248, 227, 70, 233, 171, 208, 158, 83, 51, 216, 186, 152, 8, 36, 203, 59, 252, 113, 163, 244, 85, 104, 207, 169, 25, 108, 93, 76]
for i in range(len(byte)):
    byte[i]=byte[i]^table[i]
for i in byte:
    print('0x%x'%i,end=" ")
print()
# 0x40 0x14 0x80 0xbf 0xe9 0x99 0x2e 0x60 0x2 0x1 0x6 0xa 0xff 0x4c 0x0 0x10 0x45 0xb9 0xab 0xd0 0x9e 
