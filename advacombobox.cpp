#include "advacombobox.h"

AdvaComboBox::AdvaComboBox(QWidget *parent):QComboBox(parent)
{
}

void AdvaComboBox::showPopup()
{
    Q_EMIT listShown();
    return QComboBox::showPopup();
}
