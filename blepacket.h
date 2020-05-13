#ifndef BLEPACKET_H
#define BLEPACKET_H
#include <QString>
#include <QMap>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QMetaType>
class BlePacket
{
public:
    BlePacket(uint8_t type,uint8_t length,uint8_t *adva,std::vector<unsigned char>image,bool check);
    BlePacket(){};
    int printPacket();
    QString mClock;
    QString mPDUType;
    int mPDULength;
    QString mAdva;
    QByteArray mImage;
    bool mCRCCheck;
};
Q_DECLARE_METATYPE(BlePacket);
#endif // BLEPACKET_H
