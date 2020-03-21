#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initAction();
    initToolbar();
    initCentralWidget();
}
void MainWindow::initAction()
{
    mSettingsAction=new QAction(QIcon(":/Image/icons8-radio-tower-96.png"),"Sdr",this);
    mFunctionAction=new QAction(QIcon(":/Image/icons8-redis-144.png"),"Function",this);
}
void MainWindow::initToolbar()
{
    mToolbar=new QToolBar();
    mToolbar->setMovable(0);
    mToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mToolbar->addAction(mSettingsAction);
    mToolbar->addAction(mFunctionAction);
    this->addToolBar(mToolbar);
}
void MainWindow::initCentralWidget()
{
    mCentralWidget=new QStackedWidget;
    this->setCentralWidget(mCentralWidget);
    mSettingsWidget=new SettingsWidget;
    mFunctionWidget=new FunctionWidget;
    mCentralWidget->addWidget(mSettingsWidget);
    mCentralWidget->addWidget(mFunctionWidget);
    mSignalMapper=new QSignalMapper();
    connect(mSettingsAction,SIGNAL(triggered()),mSignalMapper,SLOT(map()));
    connect(mFunctionAction,SIGNAL(triggered()),mSignalMapper,SLOT(map()));
    mSignalMapper->setMapping(mSettingsAction, 0);
    mSignalMapper->setMapping(mFunctionAction, 1);
    connect(mSignalMapper,SIGNAL(mapped(int)),mCentralWidget,SLOT(setCurrentIndex(int)));//根据Action设置当前Stack布局
    connect(mSignalMapper,SIGNAL(mapped(int)),this,SLOT(resizeCentralWidget()));//根据当前Stack布局设置窗口大小
    connect(mSettingsAction,SIGNAL(triggered()),mSettingsWidget,SLOT(openBoard()));
    connect(mSignalMapper,SIGNAL(mapped(int)),mSettingsWidget,SLOT(closeBoard()));
    connect(mFunctionAction,SIGNAL(triggered()),mFunctionWidget,SLOT(openBoard()));
    connect(mSignalMapper,SIGNAL(mapped(int)),mFunctionWidget,SLOT(closeBoard()));
    this->resizeCentralWidget();
}
void MainWindow::resizeCentralWidget()
{
    this->setFixedSize(mCentralWidget->currentWidget()->size().rwidth(),mCentralWidget->currentWidget()->size().height()+34);
}
MainWindow::~MainWindow()
{
}

