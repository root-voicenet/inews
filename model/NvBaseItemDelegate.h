#ifndef NVRSSITEMDELEGATE_H
#define NVRSSITEMDELEGATE_H

#include <QStyledItemDelegate>

class NvBaseItemDelegate : public QStyledItemDelegate
{
public:
    NvBaseItemDelegate(QObject *parent = 0);
    virtual ~NvBaseItemDelegate();

protected:
    // overwrited methods
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index ) const;
};
#endif // NVRSSITEMDELEGATE_H
