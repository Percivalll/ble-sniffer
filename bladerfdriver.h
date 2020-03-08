#ifndef BLADERFDRIVER_H
#define BLADERFDRIVER_H
#include <libbladeRF.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QObject>
typedef struct
{
    bladerf_channel channel;
    unsigned int frequency;
    unsigned int bandwidth;
    unsigned int samplerate;
    int gain;
}BladerfParameter;
typedef struct
{
    void **buffers; /* Transmit buffers */
    size_t num_buffers; /* Number of buffers */
    size_t samples_per_buffer; /* Number of samples per buffer */
    unsigned int idx; /* The next one that needs to go out */
}BladerfData;

class BladerfDriver:public QObject
{
    Q_OBJECT
public:
    BladerfDriver();
    ~BladerfDriver();
    int configureParameter();
    QString *getBoardName();
    int openBoard();
    int closeBoard();
    QString *readLog();
private:
    void printLog(const char* log);
    BladerfParameter *mParameter;
    BladerfData *mData;
    struct bladerf *mDevice;
    struct bladerf_stream *mStream;
    struct bladerf_devinfo *mDevInfo;
    QString *mBoardName;
    QString *mLog;
signals:
    void triggered(int index);

};

#endif // BLADERFDRIVER_H
