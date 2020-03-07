#ifndef TFWIDGET_H
#define TFWIDGET_H

#include <QWidget>

namespace Ui {
class TfWidget;
}

class TfWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TfWidget(QWidget *parent = nullptr);
    ~TfWidget();

private:
    Ui::TfWidget *ui;
};

#endif // TFWIDGET_H
