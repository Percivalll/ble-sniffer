#include "bladerfDriver.h"
#include <sys/time.h>
int bladerfDriver::configureChannel(struct bladerf *dev, struct channelConfig *c)
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
struct bladerf *bladerfDriver::setBoard()
{
    int status;
    struct channelConfig config;
    struct bladerf *dev = NULL;
    struct bladerf_devinfo dev_info;
    bladerf_init_devinfo(&dev_info);
    status = bladerf_open_with_devinfo(&dev, &dev_info);
    if (status != 0)
    {
        fprintf(stderr, "Unable to open device: %s\n",

                bladerf_strerror(status));
        return NULL;
    }
    config.channel = BLADERF_CHANNEL_RX(0);
    config.frequency = 2402e6;
    config.bandwidth = 2e6;
    config.samplerate = 10e6;
    config.gain = 45;

    status = configureChannel(dev, &config);
    if (status != 0)
    {
        fprintf(stderr, "Failed to configure RX channel. Exiting.\n");
        return NULL;
    }
    else
    {
        printf("Bladerf Board is open.\n");
        return dev;
    }
}
void *bladerfDriver::stream_callback(struct bladerf *dev, struct bladerf_stream *stream,
                                     struct bladerf_metadata *metadata, void *samples,
                                     size_t num_samples, void *user_data)
{
    struct bladerf_data *my_data = (struct bladerf_data *)user_data;
    size_t i;
    int16_t *sample = (int16_t *)samples;
    static FILE *fp = fopen("Binary", "wb");
    timeval clockt;
    gettimeofday(&clockt, NULL);
    static int64_t temp = 0;
    std::cout << 1000000 * (clockt.tv_sec) + (clockt.tv_usec) - temp << std::endl;
    temp = 1000000 * (clockt.tv_sec) + (clockt.tv_usec);
    for (i = 0; i < num_samples; i++)
    {
        fwrite(sample, sizeof(int16_t), 2, fp);
        sample += 2;
    }

    void *rv = my_data->buffers[my_data->idx];
    my_data->idx = (my_data->idx + 1) % my_data->num_buffers;
    return rv;
}

struct bladerf_stream *bladerfDriver::configureStream(struct bladerf *dev, struct bladerf_stream *stream, struct bladerf_data *data)
{
    // 初始化流以供异步线程使用。此函数将在内部分配数据缓冲区，该数据缓冲区数据将在回调函数中提供给API用户。
    // 在buffers输出参数填充一个指向分配的缓冲区的列表。这允许API用户实现最适合其特定用例的缓冲区管理方案。
    // 通常，人们希望将buffers参数设置为大于该num_transfers参数的值，并跟踪当前正在“运行中”的缓冲区以及可使用的缓冲区。
    // num_transfers和buffer_size值的选择应基于期望的采样率与经由指定的流的超时值bladerf_set_stream_timeout（）,默认为1秒。
    // 对于给定的采样率，必须保持以下关系才能发送或接收数据，而不会发生超时或数据丢失。
    // 其中采样率以每秒采样数为单位，超时以秒为单位。为了解决一般系统开销，建议将右侧乘以1.1到1.25。
    // 虽然增加可用缓冲区的数量可提供额外的可靠性，但是请注意，这也会增加延迟。
    // ————————————EN——————————————————
    // Initialize a stream for use with asynchronous routines.
    // This function will internally allocate data buffers, which will be provided to the API user in callback functions.
    // The buffers output parameter populates a pointer to the list of allocated buffers. This allows the API user to implement a buffer management scheme to best suit his or her specific use case.
    // Generally, one will want to set the buffers parameter to a value larger than the num_transfers parameter, and keep track of which buffers are currently "in-flight", versus those available for use.
    // For example, for a transmit stream, modulated data can be actively written into free buffers while transfers of other buffers are occurring. Once a buffer has been filled with data, it can be marked 'in-flight' and be returned in a successive callback to transmit.
    // The choice of values for the num_transfers and buffer_size should be made based upon the desired samplerate, and the stream timeout value specified via bladerf_set_stream_timeout(), which defaults to 1 second.
    // For a given sample rate, the below relationship must be upheld to transmit or receive data without timeouts or dropped data.
    // \[ Sample\ Rate > \frac{\#\ Transfers}{Timeout} \times Buffer\ Size \]
    // ...where Sample Rate is in samples per second, and Timeout is in seconds.
    // To account for general system overhead, it is recommended to multiply the righthand side by 1.1 to 1.25.
    // While increasing the number of buffers available provides additional elasticity, be aware that it also increases latency.
    int status = bladerf_init_stream(&stream, dev, stream_callback, &(data->buffers), data->num_buffers, BLADERF_FORMAT_SC16_Q11, data->samples_per_buffer, data->num_buffers, data);
    if (status != 0)
    {
        fprintf(stderr, "Failed to init stream: %s\n",
                bladerf_strerror(status));
        bladerf_close(dev);
        return NULL;
    }
    else
    {
        fprintf(stdout, "Init stream: %s\n",
                bladerf_strerror(status));
    }
    bladerf_set_stream_timeout(dev, BLADERF_RX, 100);
    status = bladerf_enable_module(dev, BLADERF_MODULE_RX, true);
    if (status < 0)
    {
        fprintf(stderr, "Failed to enable module: %s\n",
                bladerf_strerror(status));
        bladerf_deinit_stream(stream);
        bladerf_close(dev);
        return NULL;
    }
    else
    {
        fprintf(stdout, "enable module true: %s\n",
                bladerf_strerror(status));
    }
    // bladerf_stream(stream, BLADERF_RX_X1);
    // std::thread streaming(bladerf_stream, stream, BLADERF_RX_X1);
    // streaming.join();
    return stream;
}