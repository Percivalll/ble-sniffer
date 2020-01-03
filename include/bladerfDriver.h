#include <libbladeRF.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
// namespace bladerf
// {
    struct channelConfig
    {
        bladerf_channel channel;
        unsigned int frequency;
        unsigned int bandwidth;
        unsigned int samplerate;
        int gain;
    };
    int configureChannel(struct bladerf *dev, struct channelConfig *c);
    int setBoard();
// };