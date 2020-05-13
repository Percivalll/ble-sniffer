#include "functionwidget.h"
#include "ui_functionwidget.h"

FunctionWidget::FunctionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionWidget)
{
    ui->setupUi(this);
    this->setFixedSize(1000,600);
    mRefresh=new QTimer(this);
    mBoard=new BladerfDriver;
    mModel=new QSqlQueryModel(this);
    ImageDelegate *idelegate=new ImageDelegate(this);
    mAdvaComboBox=new AdvaComboBox(this);

    ui->formLayout->setWidget(0,QFormLayout::FieldRole,mAdvaComboBox);
    ui->mPagesJumpLineEdit->setText(0);

    mDatabase=QSqlDatabase::addDatabase("QSQLITE","toRead");
    mDatabase.setDatabaseName("packetDB");
    mDatabase.open();
    QSqlQuery("create table if not exists packetTable (id integer primary key autoincrement,adva vchar not null,time text not null,type vchar not null,length int not null,image blob not null,crc int not null);",mDatabase);
    mModel->setQuery("select * from packetTable limit 0,10",mDatabase);

    ui->mTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mTableView->setModel(mModel);
    ui->mTableView->setItemDelegateForColumn(5,idelegate);
    ui->mTableView->verticalHeader()->setDefaultSectionSize(50);
    ui->mTableView->setColumnWidth(0,50);
    ui->mTableView->setColumnWidth(1,115);
    ui->mTableView->setColumnWidth(2,160);
    ui->mTableView->setColumnWidth(3,120);
    ui->mTableView->setColumnWidth(4,50);
    ui->mTableView->setColumnWidth(5,50);
    ui->mTableView->setColumnWidth(6,50);

    mAdvaComboBox->insertItem(0,"ALL");
    ui->mStatusComboBox->addItem("ALL");
    ui->mStatusComboBox->addItem("Passed");
    ui->mStatusComboBox->addItem("Failed");
    refreshAdvaComboBox();
    ui->mPagesJumpLineEdit->setText("0");

    QSignalMapper *mapper=new QSignalMapper(this);
    connect(ui->mPagesFrontPushButton,SIGNAL(clicked()),mapper,SLOT(map()));
    connect(ui->mPagesNextPushButton,SIGNAL(clicked()),mapper,SLOT(map()));
    connect(ui->mPagesJumpPushButton,SIGNAL(clicked()),mapper,SLOT(map()));
    mapper->setMapping(ui->mPagesFrontPushButton,0);
    mapper->setMapping(ui->mPagesNextPushButton,1);
    mapper->setMapping(ui->mPagesJumpPushButton,2);
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(changePages(int)));

    connect(mAdvaComboBox,SIGNAL(activated(int)),this,SLOT(resetPages()));
    connect(ui->mStatusComboBox,SIGNAL(activated(int)),this,SLOT(resetPages()));
    connect(mAdvaComboBox,SIGNAL(activated(int)),this,SLOT(setModelQuery()));
    connect(ui->mStatusComboBox,SIGNAL(activated(int)),this,SLOT(setModelQuery()));
    connect(mAdvaComboBox,SIGNAL(listShown()),this,SLOT(refreshAdvaComboBox()));
    connect(mRefresh,SIGNAL(timeout()),this,SLOT(refreshModel()));
    connect(mRefresh,SIGNAL(timeout()),this,SLOT(refreshLcd()));
    connect(mRefresh,SIGNAL(timeout()),this,SLOT(refreshPages()));
    mRefresh->start(1000);
}

FunctionWidget::~FunctionWidget()
{
    mDatabase.close();
    delete mBoard;
    delete mModel;
    delete ui;
}
void FunctionWidget::refreshAdvaComboBox()
{
    mAdvaComboBox->clear();
    mAdvaComboBox->addItem("ALL");
    QSqlQuery sql("select distinct adva from packetTable",mDatabase);
    sql.first();
    mAdvaComboBox->addItem(sql.record().value(0).toString());
    while(sql.next())
    {
        mAdvaComboBox->addItem(sql.record().value(0).toString());
    }
}
void FunctionWidget::refreshModel()
{
    mModel->setQuery(mModel->query().lastQuery(),mDatabase);
    ui->mTableView->setColumnWidth(0,50);
    ui->mTableView->setColumnWidth(1,115);
    ui->mTableView->setColumnWidth(2,160);
    ui->mTableView->setColumnWidth(3,120);
    ui->mTableView->setColumnWidth(4,50);
    ui->mTableView->setColumnWidth(5,50);
    ui->mTableView->setColumnWidth(6,50);
}
void FunctionWidget::refreshLcd()
{
    QSqlQuery sql("select count(*) from packetTable",mDatabase);
    sql.first();
    int allRecord=sql.value(0).toInt();
    sql.clear();
    sql.exec("select count(*) from packetTable where crc = 1");
    sql.first();
    int passRecord=sql.value(0).toInt();
    ui->mReceivedLcdNumber->display(allRecord);
    ui->mPassedLcdNumber->display(passRecord);
    ui->mFailedLcdNumber->display(allRecord-passRecord);
}
void FunctionWidget::refreshPages()
{
    QSqlQuery sql(mDatabase);
    if(mAdvaComboBox->currentText()=="ALL")
    {  if(ui->mStatusComboBox->currentText()=="ALL")
            sql.exec("select count(*) from packetTable");
        else if (ui->mStatusComboBox->currentText()=="Passed")
            sql.exec("select count(*) from packetTable where crc = 1");
        else if (ui->mStatusComboBox->currentText()=="Failed")
            sql.exec("select count(*) from packetTable where crc = 0")   ;
    }
    else
    {  if(ui->mStatusComboBox->currentText()=="ALL")
            sql.exec("select count(*) from packetTable where adva = '"+mAdvaComboBox->currentText()+"'");
        else if (ui->mStatusComboBox->currentText()=="Passed")
            sql.exec("select count(*) from packetTable where adva = '"+mAdvaComboBox->currentText()+"' and crc = 1");
        else if (ui->mStatusComboBox->currentText()=="Failed")
            sql.exec("select count(*) from packetTable where adva = '"+mAdvaComboBox->currentText()+"' and crc = 0");
    }
    sql.first();
    ui->mPagesCurrentLineEdit->setText(QString().number(sql.value(0).toInt()/10));
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
    threadDemod->start();
    threadStream->start();
}
void FunctionWidget::closeBoard()
{
    mBoard->closeBoard();
}
void FunctionWidget::setModelQuery()
{
    QString offset( " limit "+QString::number(ui->mPagesJumpLineEdit->text().toUInt()*10)+",10");
    if(mAdvaComboBox->currentText()=="ALL")
    {  if(ui->mStatusComboBox->currentText()=="ALL")
            mModel->setQuery("select * from packetTable"+offset,mDatabase);
        else if (ui->mStatusComboBox->currentText()=="Passed"+offset)
            mModel->setQuery("select * from packetTable where crc = 1"+offset,mDatabase);
        else if (ui->mStatusComboBox->currentText()=="Failed")
            mModel->setQuery("select * from packetTable where crc = 0"+offset,mDatabase)   ;
    }
    else
    {  if(ui->mStatusComboBox->currentText()=="ALL")
            mModel->setQuery("select * from packetTable where adva = '"+mAdvaComboBox->currentText()+"'"+offset,mDatabase);
        else if (ui->mStatusComboBox->currentText()=="Passed")
            mModel->setQuery("select * from packetTable where adva = '"+mAdvaComboBox->currentText()+"' and crc = 1"+offset,mDatabase);
        else if (ui->mStatusComboBox->currentText()=="Failed")
            mModel->setQuery("select * from packetTable where adva = '"+mAdvaComboBox->currentText()+"' and crc = 0"+offset,mDatabase);
    }
}
void FunctionWidget::changePages(int index)
{
    switch (index)
    {
    case 0:
    {
        if(ui->mPagesJumpLineEdit->text().toInt()>0)
            ui->mPagesJumpLineEdit->setText(QString::number(ui->mPagesJumpLineEdit->text().toInt()-1));
        setModelQuery();
    }
        break;
    case 1:
    {
        if(ui->mPagesJumpLineEdit->text().toInt()<ui->mPagesCurrentLineEdit->text().toInt())
            ui->mPagesJumpLineEdit->setText(QString::number(ui->mPagesJumpLineEdit->text().toInt()+1));
        setModelQuery();
    }
        break;
    case 2:
    {
        setModelQuery();
    }

    }
}
void FunctionWidget::resetPages()
{
    ui->mPagesJumpLineEdit->setText("0");
}
