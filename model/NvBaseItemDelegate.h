#ifndef NVRSSITEMDELEGATE_H
#define NVRSSITEMDELEGATE_H

#include <QItemDelegate>

class NvBaseItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit NvBaseItemDelegate(QObject *parent = 0);
    ~NvBaseItemDelegate();

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    QImage defautIcon() const;
    void setDefaultIcon(const QImage & img);
private:
    void paintHeader( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void paintObject( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void paintObjectHeader( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    QImage defaultIcon_;
signals:

public slots:

};

#endif // NVRSSITEMDELEGATE_H
