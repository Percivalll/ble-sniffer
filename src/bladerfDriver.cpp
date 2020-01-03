#include "bladerfDriver.h"

int configureChannel(struct bladerf *dev, struct channelConfig *c)
{
    int status;
    status = bladerf_set_frequency(dev, c->channel, c->frequency);
    if (status != 0)
    {
        fprintf(stderr, "Failed to set frequency = %u: %s\n", c->frequency,
                bladerf_strerror(status));
        return status;
    }
    else
    {
        printf("Successed to set frequency = %u\n", c->frequency);
    }
    
    status = bladerf_set_sample_rate(dev, c->channel, c->samplerate, NULL);
    if (status != 0)
    {
        fprintf(stderr, "Failed to set samplerate = %u: %s\n", c->samplerate,
                bladerf_strerror(status));
        return status;
    }
    else
    {
        printf("Successed to set samplerate = %u\n", c->samplerate);
    }
    status = bladerf_set_bandwidth(dev, c->channel, c->bandwidth, NULL);
    if (status != 0)
    {
        fprintf(stderr, "Failed to set bandwidth = %u: %s\n", c->bandwidth,
                bladerf_strerror(status));
        return status;
    }
     else
    {
        printf("Successed to set bandwidth = %u\n", c->samplerate);
    }
    status = bladerf_set_gain(dev, c->channel, c->gain);
    if (status != 0)
    {
        fprintf(stderr, "Failed to set gain: %s\n", bladerf_strerror(status));
        return status;
    }
      else
    {
        printf("Successed to set gain = %u\n", c->gain);
    }
    return status;
}
int setBoard()
{
    int status;
    struct channelConfig config;
    struct bladerf *dev = NULL;
    struct bladerf_devinfo dev_info;
    struct bladerf_stream *stream;
    bladerf_init_devinfo(&dev_info);
    status = bladerf_open_with_devinfo(&dev, &dev_info);
    if (status != 0)
    {
        fprintf(stderr, "Unable to open device: %s\n",
        
                bladerf_strerror(status));
        return 1;
    }
    config.channel = BLADERF_CHANNEL_RX(0);
    config.frequency = 2402e6;
    config.bandwidth = 2e6;
    config.samplerate = 10e6;
    config.gain = 40;

    status = configureChannel(dev, &config);
    if (status != 0)
    {
        fprintf(stderr, "Failed to configure RX channel. Exiting.\n");
    }
    else
    printf("Bladerf Board is open.\n");
}