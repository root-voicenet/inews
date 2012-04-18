#include "NvBaseItemDelegate.h"
#include <QPainter>
#include <QStyle>
#include <QDate>
#include <QDebug>
#include <QApplication>

#include "NvAbstractListItem.h"
#include "NvObjectModel.h"

#define ICON_SIZE 32
#define OFFSET_H 5
#define CHECK_WIDTH 50
#define PROGRESS_HEIGHT 21
#define OFFSET_BUTTON 5
#define DETAIL_OFFSET 30

#define ITEM_HEIGHT 64
#define HEADER_HEIGHT 24

#define STRIPE_WIDTH 30

NvBaseItemDelegate::NvBaseItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

NvBaseItemDelegate::~NvBaseItemDelegate()
{

}

void NvBaseItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        painter->save();

        if(index.parent().isValid())
        {
                paintObject(painter, option, index);
        } else
        {
                paintHeader(painter, option, index);
        }
        painter->restore();
        painter->save();
        painter->setPen(QColor(0xD7, 0xD7, 0xD7));
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        painter->restore();
}

QSize NvBaseItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.parent().isValid()) {
       return QSize(-1, HEADER_HEIGHT);
    } else {
        int height_ = ITEM_HEIGHT;
       return QSize(-1, height_);
    }
}

void NvBaseItemDelegate::setDefaultIcon(const QImage &img)
{
    defaultIcon_ = img;
}

QImage NvBaseItemDelegate::defautIcon() const
{
    return defaultIcon_;
}

void NvBaseItemDelegate::paintHeader(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPainter &p = *painter;
    p.save();
    p.setClipRect(option.rect);
    p.setPen(QColor(77, 77, 77));
    // ðèñóåì òåêñò
    QRect tr;
    QString name = index.data(Qt::DisplayRole).toString(),
            desc = index.data(NvObjectModel::DetailRole).toString();

    QFont f = option.font;
    f.setPointSize(12);
    f.setWeight(QFont::Bold);
    QFontMetrics fm(f);
    tr = fm.boundingRect(name);
    p.setFont(f);
    p.drawText(option.rect, Qt::AlignVCenter | Qt::AlignLeft, name);

    f = option.font;
    f.setWeight(QFont::DemiBold);
    p.setFont(f);
    p.drawText(option.rect, Qt::AlignVCenter | Qt::AlignRight, desc);

    p.restore();
}

void NvBaseItemDelegate::paintObject(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect tr;
    QString name = index.data(Qt::DisplayRole).toString(),
                    description = index.data(NvObjectModel::DescriptionRole).toString();
    QPainter &p = *painter;
    p.setClipRect(option.rect);

    p.setPen(QColor(210, 210, 210));
    p.setBrush(QColor(240, 240, 240));

    p.setPen(QColor(77, 77, 77));
    p.translate(option.rect.topLeft());

    p.translate(0, sizeHint(option, index).height() - ITEM_HEIGHT);

    p.translate(OFFSET_H, OFFSET_H);

    /*
    QImage img = index.data(Qt::DecorationRole).value<QImage>();
    if(!img.isNull())
    {
            p.drawImage(0,0, img);
    } else {
            p.drawImage(0,0, defaultIcon_);
    }

    p.translate(ICON_SIZE + OFFSET_H, 0);
    */

    QFont f = option.font;
    f.setPointSize(10);
    f.setWeight(QFont::Bold);
    QFontMetrics fm(f);
    tr = fm.boundingRect(name);
    p.setFont(f);
    p.drawText(0, tr.height()-5, name);

    // ðèñóåì îïèñ� íèå

    p.setFont(option.font);
    fm = QFontMetrics(option.font);

    QDateTime date_ = index.data(NvObjectModel::DateRole).toDateTime();

    int maxWidth(option.rect.width() - ICON_SIZE - OFFSET_H - DETAIL_OFFSET );
    if(!index.data(NvObjectModel::DetailRole).toString().isEmpty())
    {
            maxWidth -= fm.boundingRect(QCoreApplication::translate("list", "Detail")).width();
    }

    description = fm.elidedText(description, Qt::ElideRight, maxWidth);
    p.translate(0, ICON_SIZE / 2);
    tr = fm.boundingRect(description);
    p.drawText(0, tr.height(), description);

    if(!date_.isNull()) {
        p.translate(0, 10);
        p.drawText(0, tr.height(), date_.toString(Qt::DefaultLocaleShortDate));
    }
}
