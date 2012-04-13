#ifndef RSSLISTITEMDELEGATE_H
#define RSSLISTITEMDELEGATE_H

#include <QStyledItemDelegate>

class RssListItemDelegate : public QStyledItemDelegate
{
public:
    RssListItemDelegate();
    virtual ~RssListItemDelegate();

    // how many adta included to list item
    enum DataRole {
        HeaderTextRole = Qt::UserRole + 100,
        DescriptionRole = Qt::UserRole + 101,
        DateRole = Qt::UserRole + 102,
        IconRole = Qt::UserRole + 103
    };

protected:
    // overwrited methods
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index ) const;
};

#endif // RSSLISTITEMDELEGATE_H
