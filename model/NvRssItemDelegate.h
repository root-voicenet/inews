#ifndef NVRSSITEMDELEGATE_H
#define NVRSSITEMDELEGATE_H

#include <QStyledItemDelegate>

class NvRssItemDelegate : public QStyledItemDelegate
{
public:
    NvRssItemDelegate(QObject *parent = 0);
    virtual ~NvRssItemDelegate();

protected:
    // overwrited methods
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index ) const;
};
#endif // NVRSSITEMDELEGATE_H
