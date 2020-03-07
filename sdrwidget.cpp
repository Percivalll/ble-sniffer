#include "sdrwidget.h"
#include "ui_sdrwidget.h"

SdrWidget::SdrWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SdrWidget)
{
    ui->setupUi(this);
}

SdrWidget::~SdrWidget()
{
    delete ui;
}
