#include "bladerfdriver.h"

BladerfDriver::BladerfDriver():QObject()
{
    mDevice=NULL;
    mDevInfo=new struct bladerf_devinfo;
    mBoardName=new QString("Unconnected");
    mLog=new QString();
    mParameter=new BladerfParameter;
    mParameter->channel = BLADERF_CHANNEL_RX(0);
    mParameter->frequency = 2402e6;
    mParameter->bandwidth = 2e6;
    mParameter->samplerate = 10e6;
    mParameter->gain = 30;
    mData=new BladerfData;
    mData->idx=0;
    mData->num_buffers=2;
    mData->samples_per_buffer = 10240;//10240 samples = ~1ms for 10Mbps;
}
BladerfDriver::~BladerfDriver()
{
    delete mDevInfo;
    delete mBoardName;
    delete mParameter;
}
BladerfParameter BladerfDriver::getParameter()
{
    bladerf_get_gain(mDevice,BLADERF_RX,(bladerf_gain*)&mParameter->gain);
    bladerf_get_bandwidth(mDevice,BLADERF_RX,(bladerf_bandwidth*)&mParameter->bandwidth);
    bladerf_get_sample_rate(mDevice,BLADERF_RX,(bladerf_sample_rate*)&mParameter->samplerate);
    bladerf_get_frequency(mDevice,BLADERF_RX,(bladerf_frequency*)&mParameter->frequency);
    return *mParameter;
}
QString BladerfDriver::readLog()
{
    return *mLog;
}
QString BladerfDriver::getBoardName()
{
    return *mBoardName;
}
int BladerfDriver::configureParameter(int channel,double frequency,double gain,double samplerate,double bandwidth)
{
    int status;
    if(channel==0)
        mParameter->channel=BLADERF_RX;
    else
        mParameter->channel=BLADERF_TX;
    printLog("Successed to set channel.");
    mParameter->frequency=frequency*1000000;
    status = bladerf_set_frequency(mDevice, mParameter->channel, mParameter->frequency);
    if (status != 0)
    {
        printLog("Failed to set frequency.");
        return status;
    }
    else
    {
        printLog("Successed to set frequency");
    }
    mParameter->samplerate=samplerate*1000000;
    status = bladerf_set_sample_rate(mDevice, mParameter->channel, mParameter->samplerate, NULL);
    if (status != 0)
    {
        printLog("Failed to set samplerate.");
        return status;
    }
    else
    {
        printLog("Successed to set samplerate.");
    }
    mParameter->bandwidth=bandwidth*1000000;
    status = bladerf_set_bandwidth(mDevice, mParameter->channel, mParameter->bandwidth, NULL);
    if (status != 0)
    {
        printLog("Failed to set bandwidth.");
        return status;
    }
    else
    {
        printLog("Successed to set bandwidth.");
    }
    mParameter->gain=gain;
    status = bladerf_set_gain(mDevice, mParameter->channel, mParameter->gain);
    if (status != 0)
    {
        printLog("Failed to set gain.");
        return status;
    }
    else
    {
        printLog("Successed to set gain.");
    }
    return status;
}
int BladerfDriver::openBoard()
{
    bladerf_init_devinfo(mDevInfo);
    int status=bladerf_open_with_devinfo(&mDevice,mDevInfo);
    if(status==0)
    {
        printLog("Successed to open the board.");
        if(bladerf_is_fpga_configured(mDevice)!=1)
        {
            printLog("Failed to autoload FPGA from the board,attempt to load FPGA from current dictionary.");
            QString path=QDir::currentPath()+QString("/hostedx40-latest.rbf");
            if(bladerf_load_fpga(mDevice,path.toLatin1().data())==0)
                printLog("Successed to load FPGA from current run dictionary.");
            else
                printLog("Failed to load FPGA from current run dictionary.");
        }
        *mBoardName=QString(bladerf_get_board_name(mDevice));
        bladerf_get_frequency(mDevice,BLADERF_RX,(bladerf_frequency *)&mParameter->frequency);
        bladerf_get_gain(mDevice,BLADERF_RX,(bladerf_gain *)&mParameter->gain);
        bladerf_get_bandwidth(mDevice,BLADERF_RX,(bladerf_bandwidth *)&mParameter->bandwidth);
        bladerf_get_sample_rate(mDevice,BLADERF_RX,(bladerf_sample_rate *)&mParameter->samplerate);
        Q_EMIT triggered(3);
        return 0;
    }
    else
    {
        printLog("Failed to open the board.");
        *mBoardName=QString("Unconnected");
        Q_EMIT triggered(3);
        return status;
    }
}
int BladerfDriver::closeBoard()
{
    if(mDevice!=NULL)
    {
        printLog("Successed to close board.");
        bladerf_close(mDevice);
        mDevice=NULL;
    }
    Q_EMIT triggered(4);
    return 0;
}
int BladerfDriver::configureStream()
{
    int status = bladerf_init_stream(&mStream, mDevice, streamCallback, &(mData->buffers), mData->num_buffers, BLADERF_FORMAT_SC16_Q11, mData->samples_per_buffer, mData->num_buffers / 2, mData);
    if (status != 0)
    {
        printLog("Failed to init stream.\n");
        return -1;
    }
    else
    {
        printLog("Init stream.");
    }
    if(mParameter->channel==BLADERF_RX)
    {
        bladerf_set_stream_timeout(mDevice,BLADERF_RX, 100);
        status = bladerf_enable_module(mDevice, BLADERF_MODULE_RX, true);
        if (status < 0)
        {
            printLog("Failed to enable module.\n");
            bladerf_deinit_stream(mStream);
            bladerf_close(mDevice);
            return -1;
        }
        else
        {
            printLog("enable module true.\n");
        }
    }
    else
    {
        bladerf_set_stream_timeout(mDevice,BLADERF_TX, 100);
        status = bladerf_enable_module(mDevice, BLADERF_MODULE_TX, true);
        if (status < 0)
        {
            printLog("Failed to enable module.\n");
            bladerf_deinit_stream(mStream);
            bladerf_close(mDevice);
            return -1;
        }
        else
        {
            printLog("enable module true.\n");
        }
    }

    return 0;
}
void BladerfDriver::startStream()
{
    bladerf_stream(mStream,BLADERF_RX_X1);
}
void BladerfDriver::stopStream()
{
    bladerf_deinit_stream(mStream);
    mStream=NULL;
}
int BladerfDriver::setBuffer(DataBuffer *buffer)
{
    mData->samples_buffer=buffer;
    return 0;
}
void BladerfDriver::printLog(const char *log)
{
    mLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"));
    mLog->append(": ");
    mLog->append(log);
    mLog->append("\n");
    Q_EMIT triggered(1);
}
void* BladerfDriver::streamCallback(struct bladerf *dev, struct bladerf_stream *stream, struct bladerf_metadata *meta, void *samples, size_t num_samples, void *user_data)
{
    BladerfData* my_data=(BladerfData*)user_data;
    my_data->samples_buffer->write((int16_t*)samples);
    void *rv = my_data->buffers[my_data->idx];
    my_data->idx = (my_data->idx + 1) % my_data->num_buffers;
    return rv;
}
