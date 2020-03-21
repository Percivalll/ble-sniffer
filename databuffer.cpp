#include "databuffer.h"
DataBuffer::DataBuffer(int size,int length)
{
    mBuffer=new int16_t*[size];
    for(int i=0;i<size;i++)
        mBuffer[i]=new int16_t[length];
    mEmpty=new QSemaphore(size);
    mFull=new QSemaphore(0);
    mSize=size;
    mLength=length;
}
DataBuffer::~DataBuffer()
{
    for(int i=0;i<mSize;i++)
        delete mBuffer[i];
    delete mBuffer;
    delete mEmpty;
    delete mFull;
}

int DataBuffer::write(int16_t *input)
{
    mEmpty->acquire();
    memcpy(mBuffer[mReadIndex],input,mLength);
    mReadIndex=(mReadIndex+1)%mSize;
    mFull->release();
    return 0;
}

int DataBuffer::read(int16_t *output)
{
    mFull->acquire();
    memcpy(mBuffer[mWriteIndex],output,mLength);
    mWriteIndex=(mWriteIndex+1)%mSize;
    mEmpty->release();
    return 0;
}
