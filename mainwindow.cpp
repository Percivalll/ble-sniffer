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
    mSdrAction=new QAction(QIcon(":/Image/icons8-radio-tower-96.png"),"Sdr",this);
    mRedisAction=new QAction(QIcon(":/Image/icons8-redis-144.png"),"Redis",this);
    mTfAction=new QAction(QIcon(":/Image/tensorflow.png"),"Tensorflow",this);
}
void MainWindow::initToolbar()
{
    mToolbar=new QToolBar();
    mToolbar->setMovable(0);
    mToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mToolbar->addAction(mSdrAction);
    mToolbar->addAction(mRedisAction);
    mToolbar->addAction(mTfAction);
    this->addToolBar(mToolbar);
}
void MainWindow::initCentralWidget()
{
    mCentralWidget=new QStackedWidget;
    this->setCentralWidget(mCentralWidget);
    mSdrWidget=new SdrWidget;
    mRedisWidget=new QLabel("YES");
    mTfWidget=new TfWidget();
    mCentralWidget->addWidget(mSdrWidget);
    mCentralWidget->addWidget(mRedisWidget);
    mCentralWidget->addWidget(mTfWidget);
    mSignalMapper=new QSignalMapper();
    connect(mSdrAction,SIGNAL(triggered()),mSignalMapper,SLOT(map()));
    connect(mRedisAction,SIGNAL(triggered()),mSignalMapper,SLOT(map()));
    connect(mTfAction,SIGNAL(triggered()),mSignalMapper,SLOT(map()));
    mSignalMapper->setMapping(mSdrAction, 0);
    mSignalMapper->setMapping(mRedisAction, 1);
    mSignalMapper->setMapping(mTfAction, 2);
    connect(mSignalMapper,SIGNAL(mapped(int)),mCentralWidget,SLOT(setCurrentIndex(int)));
    connect(mSignalMapper,SIGNAL(mapped(int)),this,SLOT(resizeCentralWidget()));
    connect(mSdrAction,SIGNAL(triggered()),mSdrWidget,SLOT(openBoard()));
    connect(mSignalMapper,SIGNAL(mapped(int)),mSdrWidget,SLOT(closeBoard()));
    this->resizeCentralWidget();
}
void MainWindow::resizeCentralWidget()
{
    this->setFixedSize(mCentralWidget->currentWidget()->size().rwidth(),mCentralWidget->currentWidget()->size().height()+34);
}
MainWindow::~MainWindow()
{
}

