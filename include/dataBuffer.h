#ifndef BUFFER
#define BUFFER
#define LEN_BUF_IN_SAMPLE 10240 //10240 samples = ~1ms for 10Mbps;
#define LEN_BUF (LEN_BUF_IN_SAMPLE * 2)
#include <stdlib.h>
#include <Semaphore.h>
class dataBuffer
{
public:
    dataBuffer();
    ~dataBuffer();
    int write();
    int read();

private:
    int16_t **buffer;
    std::mutex trLock,twLock,globalLock;
    std::condition_variable toread;
    std::condition_variable towrite;
    int status[3];
};
#endif