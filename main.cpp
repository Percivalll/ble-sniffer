#include "mainwindow.h"
#include <QApplication>
#include "blepacket.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qRegisterMetaType<BlePacket>("BlePacket");
    w.show();
    return a.exec();
}
