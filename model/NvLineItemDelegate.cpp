#include "NvLineItemDelegate.h"

#include "NvAbstractListItem.h"
#include "NvObjectModel.h"

#include <QPainter>

#define ITEM_HEIGHT 16

NvLineItemDelegate::NvLineItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

NvLineItemDelegate::~NvLineItemDelegate()
{

}

void NvLineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect tr;
    QString name = index.data(Qt::DisplayRole).toString();
    QPainter &p = *painter;
    p.save();

    p.setClipRect(option.rect);

    //p.setPen(QColor(210, 210, 210));
    //p.setBrush(QColor(240, 240, 240));
    //p.translate(10, 1);


    QFont f = option.font;
    QFontMetrics fm(f);
    tr = fm.boundingRect(name);
    p.setFont(f);
    p.drawText(option.rect, Qt::AlignVCenter | Qt::AlignLeft, name);
    p.restore();
}

QSize NvLineItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(-1, ITEM_HEIGHT);
}
