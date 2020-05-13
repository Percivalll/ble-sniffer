#include "blepacket.h"

BlePacket::BlePacket(uint8_t type,uint8_t length,uint8_t *adva,std::vector<unsigned char> image,bool check)
{
    mClock=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (type)
    {
    case 0:
        mPDUType = "ADV_IND";
        break;
    case 1:
        mPDUType = "ADV_DIRECT_IND";
        break;
    case 2:
        mPDUType = "ADV_NONCONN_IND";
        break;
    case 3:
        mPDUType = "SCAN_REQ";
        break;
    case 4:
        mPDUType = "SCAN_RSP";
        break;
    case 5:
        mPDUType = "CONNECT_REQ";
        break;
    case 6:
        mPDUType = "ADV_SCAN_IND";
        break;
    default:
        mPDUType = "RESERVED";
    }
    mPDULength=length;
    mAdva=QByteArray((char*)adva,6).toHex();
    mImage=QByteArray((char*)image.data(),image.size());
    mCRCCheck=check;
}
int BlePacket::printPacket()
{
    qDebug()<<mClock<<mPDUType<<mPDULength<<mAdva<<mImage<<mCRCCheck;
    return 0;
}
