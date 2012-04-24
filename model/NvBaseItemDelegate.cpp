#include "NvBaseItemDelegate.h"
#include <QPainter>
#include <QStyle>
#include <QDate>
#include <QDebug>
#include <QApplication>

#include "NvAbstractListItem.h"
#include "NvObjectModel.h"

#define HEADER_OFFSET 30

NvBaseItemDelegate::NvBaseItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

NvBaseItemDelegate::~NvBaseItemDelegate()
{

}

void NvBaseItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFont font = QApplication::font();
    font.setBold( true );
    QStyleOptionViewItem opt = option;
    opt.displayAlignment = Qt::AlignTop | Qt::AlignLeft;
    opt.font = font;

    QStyledItemDelegate::paint(painter,opt,index);

        painter->save();
        QFont SubFont = QApplication::font();
        QFont dateFont = QApplication::font();

        //font.setPixelSize(font.weight()+);
        font.setBold(true);
        SubFont.setWeight(SubFont.weight()-2);
        QFontMetrics fm(font);
        QFontMetrics dfm(dateFont);

        QIcon icon = qvariant_cast<QIcon>(index.data(NvObjectModel::IconRole));
        QString subText = qvariant_cast<QString>(index.data(NvObjectModel::DescriptionRole));
        QString dateText = qvariant_cast<QString>(index.data(NvObjectModel::DateRole));
        bool promoted = qvariant_cast<bool>(index.data(NvObjectModel::PromotedRole));

        QSize iconsize = icon.actualSize(option.decorationSize);

        QRect subheaderRect = option.rect;
        subheaderRect.translate(0, HEADER_OFFSET);

        QRect iconRect = subheaderRect;
        QRect dateRect = subheaderRect;

        if(promoted) {
            QIcon sicon(":/images/star.png");
            QRect starRect = subheaderRect;
            QSize ss = sicon.actualSize(option.decorationSize);

            starRect.setRight(ss.width() + 10);

            iconRect.setLeft(starRect.left() + 5);

            painter->drawPixmap(QPoint(starRect.left()+ss.width()/2+2,starRect.top()),sicon.pixmap(ss.width(),ss.height()));
        }
        iconRect.setRight(iconsize.width()+30);
        iconRect.setTop(iconRect.top()+5);


        int dateWidth = 0;
        if(!dateText.isEmpty()) {
            dateWidth = dfm.width(dateText) + 5;
            dateRect.setLeft(iconRect.right());
        }


        subheaderRect.setLeft(iconRect.right() + dateWidth);


        //painter->drawPixmap(QPoint(iconRect.right()/2,iconRect.top()/2),icon.pixmap(iconsize.width(),iconsize.height()));
        painter->drawPixmap(QPoint(iconRect.left()+iconsize.width()/2+2,iconRect.top()+iconsize.height()/2+3),icon.pixmap(iconsize.width(),iconsize.height()));


        painter->setFont(SubFont);
        painter->drawText(subheaderRect.left(),subheaderRect.top(),subText);

        if(dateWidth) {
            painter->setFont(dateFont);
            painter->drawText(dateRect.left(), dateRect.top(), dateText);
        }


        painter->restore();
        painter->save();
        painter->setPen(QColor(0xD7, 0xD7, 0xD7));
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        painter->restore();
}

QSize NvBaseItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QIcon icon = qvariant_cast<QIcon>(index.data(NvObjectModel::IconRole));
    QSize iconsize = icon.actualSize(option.decorationSize);
    QFont font = QApplication::font();
    QFontMetrics fm(font);

    int height = iconsize.isEmpty() ? 30 : iconsize.height();

    return(QSize(iconsize.width(), height + fm.height() + 8));
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
