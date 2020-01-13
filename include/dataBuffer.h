#ifndef BUFFER
#define BUFFER
#include <stdlib.h>
#include <Semaphore.h>
#include <iostream>
#define LEN_BUF_IN_SAMPLE 10240 //10240 samples = ~1ms for 10Mbps;
#define LEN_BUF (LEN_BUF_IN_SAMPLE * 2)
class dataBuffer
{
public:
    dataBuffer(int size);
    ~dataBuffer();
    int write(int16_t *input);
    int read(int (*function)(int16_t *input));
    int count=0;
private:
    int16_t **buffer;
    int readIndex;
    int writeIndex;
    int cellSize;
    Semaphore *empty,*full;
    std::mutex lock;
};
#endif