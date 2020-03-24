#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QtCore>
#include <QMessageBox>
#include "bladerfdriver.h"
namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

private:
    Ui::SettingsWidget *ui;
    BladerfDriver *mBoard;
public Q_SLOTS:
    void openBoard();
    void closeBoard();
    void watchBoard(int index);
    void clearParameter();
    void setParameter();
};

#endif // SETTINGSWIDGET_H
