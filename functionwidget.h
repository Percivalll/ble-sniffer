#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QQueue>
#include <QByteArray>
#include <QThread>
#include "databuffer.h"
#include <bladerfdriver.h>
namespace Ui {
class FunctionWidget;
}

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent = nullptr);
    ~FunctionWidget();
public slots:
    void openBoard();
    void closeBoard();
private:
    Ui::FunctionWidget *ui;
    BladerfDriver *mBoard;
};

#endif // FUNCTIONWIDGET_H
