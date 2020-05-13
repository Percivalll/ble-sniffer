#ifndef ADVACOMBOBOX_H
#define ADVACOMBOBOX_H
#include <QComboBox>

class AdvaComboBox : public QComboBox
{
    Q_OBJECT
public:
    AdvaComboBox(QWidget *parent);
    void showPopup() override;
Q_SIGNALS:
    void listShown();
};

#endif // ADVACOMBOBOX_H
