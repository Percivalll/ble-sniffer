#ifndef DATABUFFER_H
#define DATABUFFER_H
#include <QByteArray>
#include <QVector>
#include <QSemaphore>
#include <QDebug>
#include <sys/time.h>
class DataBuffer
{
public:
    DataBuffer(int size,int length);
    ~DataBuffer();
    int write(int16_t *input);
    int read(int16_t *output);
    int getSize(){return mSize;};
    int getLength(){return mLength;};
private:
    int16_t **mBuffer;
    QSemaphore *mFull,*mEmpty;
    int mReadIndex=0,mWriteIndex=0,mSize=0,mLength=0;
};

#endif // DATABUFFER_H
