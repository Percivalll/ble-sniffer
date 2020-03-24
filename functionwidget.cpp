#include "functionwidget.h"
#include "ui_functionwidget.h"

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);
    this->setFixedSize(700,600);
    mBoard=new BladerfDriver;
    mDatabase=QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName("packetDB");
    mDatabase.open();
    QSqlQuery("create table if not exists packetTable (id integer primary key autoincrement,adva vchar not null,time text not null,type vchar not null,length int not null,raw vchar not null,crc int not null);",mDatabase);
    mModel=new QSqlTableModel(this,mDatabase);
    mModel->setTable("packetTable");
    mModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    mModel->select();
    mView=new QTableView(this);
    mView->setGeometry(40,50,615,400);
    mView->setModel(mModel);
    mView->hideColumn(0);
    mView->setColumnWidth(1,110);
    mView->setColumnWidth(2,160);
    mView->setColumnWidth(3,80);
    mView->setColumnWidth(4,40);
    mView->setColumnWidth(5,100);
    mView->setColumnWidth(6,40);
}

FunctionWidget::~FunctionWidget()
{
    delete mBoard;
    delete mModel;
    delete ui;
}
void FunctionWidget::openBoard()
{
    mBoard->openBoard();
    mBoard->configureStream();
    DataBuffer* buffer=new DataBuffer(100,10240*2);
    mBoard->setBuffer(buffer);
    QThread *threadStream=new QThread;
    mBoard->moveToThread(threadStream);
    connect(threadStream,SIGNAL(started()),mBoard,SLOT(startStream()));
    mDemod=new Demodulator(mBoard->getParameter().samplerate);
    mDemod->setBuffer(buffer);
    QThread *threadDemod=new QThread;
    mDemod->moveToThread(threadDemod);
    connect(threadDemod,SIGNAL(started()),mDemod,SLOT(startDemod()));
    connect(mDemod,SIGNAL(receivedPacket(BlePacket)),this,SLOT(insertRecord(BlePacket)));
    threadDemod->start();
    threadStream->start();
}
void FunctionWidget::closeBoard()
{
    mBoard->closeBoard();
}
void FunctionWidget::insertRecord(BlePacket packet)
{
    QSqlRecord record=mModel->record();
    record.setGenerated(0,false);
    record.setValue(1,packet.mAdva);
    record.setValue(2,packet.mClock);
    record.setValue(3,packet.mPDUType);
    record.setValue(4,packet.mPDULength);
    record.setValue(5,packet.mRawData);
    record.setValue(6,packet.mCRCCheck);
    if(mModel->insertRecord(-1,record)==true)
        ui->lcdNumber->display(mModel->rowCount());
}
