#ifndef GLOBAL
#define GLOBAL
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#define LEN_BUF_IN_SAMPLE 10240 //10240 samples = ~1ms for 10Mbps;
#define LEN_BUF (LEN_BUF_IN_SAMPLE * 2)
extern bool signalExit;
extern struct bladerf *sgdev;
extern struct bladerf_stream *sgstream;
#endif