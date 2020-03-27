#ifndef BLADERFDRIVER_H
#define BLADERFDRIVER_H
#include <libbladeRF.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QTemporaryFile>
#include <QFile>
#include "databuffer.h"
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
    DataBuffer *samples_buffer;
}BladerfData;

class BladerfDriver:public QObject
{
    Q_OBJECT
public:
    BladerfDriver();
    ~BladerfDriver();
    BladerfParameter getParameter();
    int configureParameter(int channel,double frequency,double gain,double samplerate,double bandwidth);
    int configureStream();
    int openBoard();
    int closeBoard();
    int setBuffer(DataBuffer *buffer);
    QString readLog();
    QString getBoardName();
private:
    BladerfParameter *mParameter;
    BladerfData *mData;
    struct bladerf *mDevice;
    struct bladerf_stream *mStream;
    struct bladerf_devinfo *mDevInfo;
    QString *mBoardName;
    QString *mLog;
    void printLog(const char* log);
    static void* streamCallback(struct bladerf *dev, struct bladerf_stream *stream, struct bladerf_metadata *meta, void *samples, size_t num_samples, void *user_data);
public Q_SLOTS:
    void startStream();
    void stopStream();
Q_SIGNALS:
    void triggered(int index);

};

#endif // BLADERFDRIVER_H
