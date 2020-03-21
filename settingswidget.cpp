#include "settingswidget.h"
#include "ui_settingswidget.h"
SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    this->setFixedSize(800,400);
    mBoard=new BladerfDriver;
    connect(mBoard,SIGNAL(triggered(int)),this,SLOT(watchBoard(int)));
    connect(ui->mButtonBox,SIGNAL(rejected()),this,SLOT(clearParameter()));
    connect(ui->mButtonBox,SIGNAL(accepted()),this,SLOT(setParameter()));
    openBoard();
}
void SettingsWidget::openBoard()
{
    mBoard->openBoard();
}
void SettingsWidget::closeBoard()
{
    mBoard->closeBoard();
}
void SettingsWidget::watchBoard(int index)
{
    switch (index)
    {
    case 1:
        ui->mLogtextBrowser->setText(mBoard->readLog());
        break;
    case 2:
        break;
    case 3:
        ui->mBoardLineEdit->setText(mBoard->getBoardName());
        break;
    case 4:
        break;

    }
}
void SettingsWidget::clearParameter()
{
    ui->mChannelComboBox->setCurrentIndex(0);
    ui->mBandwidthDoubleSpinBox->setValue(0);
    ui->mGainDoubleSpinBox->setValue(0);
    ui->mSamplerateDoubleSpinBox->setValue(0);
    ui->mFrequencyDoubleSpinBox->setValue(0);
}
void SettingsWidget::setParameter()
{
    if(mBoard->getBoardName()=="Unconnected")
        QMessageBox::critical(NULL, "Error", "Device hasn't connected!");
    else
        mBoard->configureParameter(ui->mChannelComboBox->currentIndex(),ui->mFrequencyDoubleSpinBox->value(),ui->mGainDoubleSpinBox->value(),ui->mSamplerateDoubleSpinBox->value(),ui->mBandwidthDoubleSpinBox->value());
    mBoard->configureStream();
}
SettingsWidget::~SettingsWidget()
{
    closeBoard();
    delete ui;
    delete mBoard;
}
