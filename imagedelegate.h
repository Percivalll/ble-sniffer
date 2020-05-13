#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPixmap>
#include <QVariant>
#include <QDebug>
#include <QImage>
#include <QWidget>
#include <QPushButton>
class ImageDelegate : public QStyledItemDelegate
{
public:
    ImageDelegate(QObject *parent);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif // IMAGEDELEGATE_H
