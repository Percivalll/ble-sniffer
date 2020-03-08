#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
//#include "bladerfdriver.h"
//int main()
//{
//    BladerfDriver bd;
//    bd.openBoard();
//    bd.getBoardName();
//}
