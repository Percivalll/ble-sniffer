#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QDebug>
#include "functionwidget.h"
#include "settingswidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initAction();
    void initToolbar();
    void initCentralWidget();
    QToolBar *mToolbar;
    QActionGroup *mActionGroup;
    QAction *mSettingsAction,*mFunctionAction;
    QStackedWidget *mCentralWidget;
    QSignalMapper *mSignalMapper;
    SettingsWidget *mSettingsWidget;
    FunctionWidget *mFunctionWidget;
public slots:
    void resizeCentralWidget();
};
#endif // MAINWINDOW_H
