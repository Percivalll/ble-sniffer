#include "tfwidget.h"
#include "ui_tfwidget.h"

TfWidget::TfWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TfWidget)
{
    ui->setupUi(this);
}

TfWidget::~TfWidget()
{
    delete ui;
}
