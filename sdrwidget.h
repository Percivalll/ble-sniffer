#ifndef SDRWIDGET_H
#define SDRWIDGET_H

#include <QWidget>

namespace Ui {
class SdrWidget;
}

class SdrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SdrWidget(QWidget *parent = nullptr);
    ~SdrWidget();

private:
    Ui::SdrWidget *ui;
};

#endif // SDRWIDGET_H
