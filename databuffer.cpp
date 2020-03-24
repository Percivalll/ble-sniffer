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
    memcpy(mBuffer[mReadIndex],input,mLength*sizeof (int16_t));
    mReadIndex=(mReadIndex+1)%mSize;
    mFull->release();
    return 0;
}

int DataBuffer::read(int16_t *output)
{
//    struct timeval tpstart,tpend;
//    float timeuse;
//    gettimeofday(&tpstart,NULL);
    mFull->acquire();
    memcpy(output,mBuffer[mWriteIndex],mLength*sizeof (int16_t));
    mWriteIndex=(mWriteIndex+1)%mSize;
    mEmpty->release();
//    gettimeofday(&tpend,NULL);
//    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec);
//    qDebug()<<timeuse<<"us";
    return 0;
}
