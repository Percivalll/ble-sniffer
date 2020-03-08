#include "tfwidget.h"
#include "ui_tfwidget.h"

TfWidget::TfWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TfWidget)
{
    ui->setupUi(this);
    this->setFixedSize(800,800);
}

TfWidget::~TfWidget()
{
    delete ui;
}
