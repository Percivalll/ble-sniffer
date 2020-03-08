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
#include "sdrwidget.h"
#include "tfwidget.h"
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
    QAction *mSdrAction,*mRedisAction,*mTfAction;
    QStackedWidget *mCentralWidget;
    QSignalMapper *mSignalMapper;
    SdrWidget *mSdrWidget;
    QLabel *mRedisWidget;
    TfWidget *mTfWidget;
public slots:
    void resizeCentralWidget();
};
#endif // MAINWINDOW_H
