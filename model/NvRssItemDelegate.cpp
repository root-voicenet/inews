#include "NvRssItemDelegate.h"
#include <QPainter>
#include <QStyle>
#include <QDate>
#include <QDebug>
#include <QApplication>
#include <QTextDocument>

#include "NvAbstractListItem.h"
#include "NvRssCachedModel.h"

#define HEADER_OFFSET 15
#define ITEM_HEIGHT 70;

NvRssItemDelegate::NvRssItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

NvRssItemDelegate::~NvRssItemDelegate()
{

}

void NvRssItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFont font = option.font;
    font.setBold( true );
    QStyleOptionViewItem opt = option;
    opt.displayAlignment = Qt::AlignTop | Qt::AlignLeft;
    opt.font = font;

    QStyledItemDelegate::paint(painter,opt,index);

    painter->save();
    painter->translate(option.rect.topLeft());

    QFont dateFont = QApplication::font();
    QRect tr;
    int offset = HEADER_OFFSET;

        //font.setPixelSize(font.weight()+);
        font.setBold( false );
        dateFont.setBold( true );
        QFontMetrics dfm(dateFont);

        QString subText = qvariant_cast<QString>(index.data(NvRssCachedModel::DescriptionRole));
        QDateTime date = qvariant_cast<QDateTime>(index.data(NvRssCachedModel::DateRole));
        QString dateText = date.toString("dd.MM hh:mm");
        QString tags = qvariant_cast<QString>(index.data(NvRssCachedModel::TagRole));


        int dateWidth = 0;
        if(!dateText.isEmpty()) {
            dateWidth = dfm.width(dateText) + 5;
        }

        QFontMetrics fm(option.font);
        if(!subText.isEmpty()) {
            tr = fm.boundingRect( subText );
            tr.moveTo( 0, offset );
            offset += tr.height() + 2;
            painter->setFont(option.font);
            painter->drawText(tr, Qt::TextWordWrap | Qt::TextWrapAnywhere, subText);
        }

        int dateOffset = 0;
        if(!dateText.isEmpty()) {
            tr = dfm.boundingRect( dateText );
            tr.moveTo( 0, offset );
            painter->setFont( dateFont );
            painter->drawText(tr, dateText);
            dateOffset = tr.width() + 5;
        }

        QString feedName = qvariant_cast<QString>(index.data(NvRssCachedModel::FeedRole));
        if(!feedName.isEmpty()) {
            tr = fm.boundingRect( feedName );
            tr.moveTo(dateOffset, offset);
            offset += tr.height() + 2;
            painter->setPen(QColor(0, 0, 210));
            painter->setFont( font );
            painter->drawText(tr, feedName);
        }

        if(!tags.isEmpty()) {
            tr = fm.boundingRect( tags );
            tr.moveTo( 0, offset);
            offset += tr.height() + 2;
            painter->setPen(QColor(210, 150, 0));
            painter->drawText(tr, tags );
        }

        painter->restore();
        painter->save();
        painter->setPen(QColor(0xD7, 0xD7, 0xD7));
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        painter->restore();
}

QSize NvRssItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int height = ITEM_HEIGHT;

    return(QSize(0, height));
}

/*
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

*/
