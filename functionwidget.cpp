#include "functionwidget.h"
#include "ui_functionwidget.h"

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);
    this->setFixedSize(600,400);
    mBoard=new BladerfDriver;
}

FunctionWidget::~FunctionWidget()
{
    delete ui;
}
void FunctionWidget::openBoard()
{
    mBoard->openBoard();
    mBoard->configureStream();
    DataBuffer* buffer=new DataBuffer(10,10240*2);
    mBoard->setBuffer(buffer);
    QThread *thread=new QThread;
    mBoard->moveToThread(thread);
    connect(thread,SIGNAL(started()),mBoard,SLOT(startStream()));
    thread->start();
}
void FunctionWidget::closeBoard()
{
    mBoard->closeBoard();
}
