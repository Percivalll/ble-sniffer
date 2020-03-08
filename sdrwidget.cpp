#include "sdrwidget.h"
#include "ui_sdrwidget.h"
SdrWidget::SdrWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SdrWidget)
{
    ui->setupUi(this);
    this->setFixedSize(700,400);
    mBoard=new BladerfDriver;
    connect(mBoard,SIGNAL(triggered(int)),this,SLOT(watchBoard(int)));
    openBoard();
}
void SdrWidget::openBoard()
{
    mBoard->openBoard();
}
void SdrWidget::closeBoard()
{
    mBoard->closeBoard();
}
void SdrWidget::watchBoard(int index)
{
    switch (index)
    {
    case 1:
        ui->mLogtextBrowser->setText(*mBoard->readLog());
        break;
    case 2:
        break;
    case 3:
        ui->mBoardLineEdit->setText(*mBoard->getBoardName());
        break;
    case 4:
        break;

    }
}
SdrWidget::~SdrWidget()
{
    closeBoard();
    delete ui;
    delete mBoard;
}
