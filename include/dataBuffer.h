#ifndef BUFFER
#define BUFFER
#define LEN_BUF_IN_SAMPLE 10240 //10240 samples = ~1ms for 10Mbps;
#define LEN_BUF (LEN_BUF_IN_SAMPLE * 2)
#include <stdlib.h>
enum bufStatus
{
    READ=-1,
    WRITE=1,
    FREE=0
};
struct bufCell
{
    int16_t bc[LEN_BUF];
    enum bufStatus status;
};
class dataBuffer
{
    public:
    dataBuffer();
    struct bufCell *write();
    struct bufCell *read();
    private:
    struct bufCell buffer[3];
    struct bufCell * findCell(enum bufStatus status);
};
#endif