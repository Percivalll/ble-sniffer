#include "imagedelegate.h"

ImageDelegate::ImageDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}
void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    const QAbstractItemModel *model=index.model();
    QVariant data=model->data(index);
//    QImage image((uchar*)data.toByteArray().data(),32,32,QImage::Format_Grayscale8);
    QImage image(32,32,QImage::Format_RGB888);
    image.loadFromData(data.toByteArray());
    painter->drawImage(option.rect,image);
}

QWidget *ImageDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::createEditor(parent,option,index);
}

