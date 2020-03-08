#include "bladerfdriver.h"

BladerfDriver::BladerfDriver():QObject()
{
    mDevInfo=new struct bladerf_devinfo;
    mBoardName=new QString("Unknown");
    mLog=new QString();
    mParameter=new BladerfParameter;
}
BladerfDriver::~BladerfDriver()
{
    delete mDevInfo;
    delete mBoardName;
    delete mParameter;
}
void BladerfDriver::printLog(const char *log)
{
    mLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"));
    mLog->append(": ");
    mLog->append(log);
    mLog->append("\n");
    emit triggered(1);
}
QString* BladerfDriver::readLog()
{
    return mLog;
}
QString* BladerfDriver::getBoardName()
{
    return mBoardName;
}
int BladerfDriver::configureParameter()
{
    int status;
    status = bladerf_set_frequency(mDevice, mParameter->channel, mParameter->frequency);
    if (status != 0)
    {
        printLog("Failed to set frequency.");
        emit triggered(2);
        return status;
    }
    else
    {
        printLog("Successed to set frequency");
    }

    status = bladerf_set_sample_rate(mDevice, mParameter->channel, mParameter->samplerate, NULL);
    if (status != 0)
    {
        printLog("Failed to set samplerate.");
        emit triggered(2);
        return status;
    }
    else
    {
        printLog("Successed to set samplerate.");
    }
    status = bladerf_set_bandwidth(mDevice, mParameter->channel, mParameter->bandwidth, NULL);
    if (status != 0)
    {
        printLog("Failed to set bandwidth.");
        emit triggered(2);
        return status;
    }
    else
    {
        printLog("Successed to set bandwidth.");
    }
    status = bladerf_set_gain(mDevice, mParameter->channel, mParameter->gain);
    if (status != 0)
    {
        printLog("Failed to set gain.");
        emit triggered(2);
        return status;
    }
    else
    {
        printLog("Successed to set gain.");
    }
    emit triggered(2);
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
        emit triggered(3);
        return 0;
    }
    else
    {
        printLog("Failed to open the board.");
        *mBoardName=QString("Unconnected");
        emit triggered(3);
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
    emit triggered(4);
    return 0;
}
