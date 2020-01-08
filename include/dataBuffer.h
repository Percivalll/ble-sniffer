#ifndef BUFFER
#define BUFFER
#define LEN_BUF_IN_SAMPLE 10240 //10240 samples = ~1ms for 10Mbps;
#define LEN_BUF (LEN_BUF_IN_SAMPLE * 2)
#include <stdlib.h>
#include <Semaphore.h>
enum bufStatus
{
    FULL = 0,
    EMPTY = 1,
};
struct bufCell
{
    int16_t bc[LEN_BUF];
    enum bufStatus status;
};

class dataBuffer
{
public:
    Semaphore *emptySp;
    Semaphore *fullSp;
    dataBuffer();
    struct bufCell *producer();
    struct bufCell *consumer();
    std::mutex lock;
private:
    struct bufCell *findCell(enum bufStatus status);
    struct bufCell buffer[3];
};
#endif