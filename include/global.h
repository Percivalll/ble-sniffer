#ifndef GLOBAL
#define GLOBAL
#define LEN_BUF_IN_SAMPLE 10240 //10240 samples = ~1ms for 10Mbps;
#define LEN_BUF (LEN_BUF_IN_SAMPLE * 2)
#include <dataBuffer.h>
extern dataBuffer *rxbuf;
#endif