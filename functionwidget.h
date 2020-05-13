#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QQueue>
#include <QByteArray>
#include <QThread>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QTimer>
#include <QSignalMapper>
#include "imagedelegate.h"
#include "databuffer.h"
#include "bladerfdriver.h"
#include "demodulator.h"
#include "advacombobox.h"

namespace Ui {
class FunctionWidget;
}
class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent = nullptr);
    ~FunctionWidget();
public Q_SLOTS:
    void openBoard();
    void closeBoard();
    void refreshModel();
    void refreshAdvaComboBox();
    void refreshLcd();
    void refreshPages();
    void setModelQuery();
    void changePages(int index);
    void resetPages();
private:
    Ui::FunctionWidget *ui;
    BladerfDriver *mBoard;
    QSqlDatabase mDatabase;
    QSqlQueryModel *mModel;
    Demodulator *mDemod;
    QTimer *mRefresh;
    AdvaComboBox *mAdvaComboBox;

};

#endif // FUNCTIONWIDGET_H
