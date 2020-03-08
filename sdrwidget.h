#ifndef SDRWIDGET_H
#define SDRWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QtCore>
#include "bladerfdriver.h"
namespace Ui {
class SdrWidget;
}

class SdrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SdrWidget(QWidget *parent = nullptr);
    ~SdrWidget();

private:
    Ui::SdrWidget *ui;
    BladerfDriver *mBoard;
public slots:
    void openBoard();
    void closeBoard();
    void watchBoard(int index);
};

#endif // SDRWIDGET_H
