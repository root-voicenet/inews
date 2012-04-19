#ifndef NVLINEITEMDELEGATE_H
#define NVLINEITEMDELEGATE_H

#include <QItemDelegate>

class NvLineItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit NvLineItemDelegate(QObject *parent = 0);
    ~NvLineItemDelegate();
    
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
signals:
    
public slots:
    
};

#endif // NVLINEITEMDELEGATE_H
