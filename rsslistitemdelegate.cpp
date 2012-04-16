#include "rsslistitemdelegate.h"
#include <QtGui>

RssListItemDelegate::RssListItemDelegate()
{

}

RssListItemDelegate::~RssListItemDelegate()
{

}

//alocate each item size in listview.
QSize RssListItemDelegate::sizeHint(const QStyleOptionViewItem&  option,  const QModelIndex& index) const
{
    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QSize iconsize = icon.actualSize(option.decorationSize);
    QFont font = QApplication::font();
    QFontMetrics fm(font);

    return(QSize(iconsize.width(), iconsize.height() + fm.height() + 8));
}

void RssListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    QFont font = QApplication::font();
    QFont SubFont = QApplication::font();
    QFont dateFont = QApplication::font();

    //font.setPixelSize(font.weight()+);
    font.setBold(true);
    SubFont.setWeight(SubFont.weight()-2);
    QFontMetrics fm(font);
    QFontMetrics dfm(dateFont);

    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QString headerText = qvariant_cast<QString>(index.data(HeaderTextRole));
    QString subText = qvariant_cast<QString>(index.data(DescriptionRole));
    QString dateText = qvariant_cast<QString>(index.data(DateRole));

    QSize iconsize = icon.actualSize(option.decorationSize);

    QRect headerRect = option.rect;
    QRect subheaderRect = option.rect;
    QRect iconRect = subheaderRect;
    QRect dateRect = subheaderRect;

    iconRect.setRight(iconsize.width()+30);
    iconRect.setTop(iconRect.top()+5);
    headerRect.setLeft(iconRect.right());
    headerRect.setTop(headerRect.top()+5);
    headerRect.setBottom(headerRect.top()+fm.height());

    int dateWidth = 0;
    if(!dateText.isEmpty()) {
        dateWidth = dfm.width(dateText) + 5;
        dateRect.setLeft(iconRect.right());
        dateRect.setTop(headerRect.bottom()+17);
    }


    subheaderRect.setLeft(iconRect.right() + dateWidth);
    subheaderRect.setTop(headerRect.bottom()+17);


    //painter->drawPixmap(QPoint(iconRect.right()/2,iconRect.top()/2),icon.pixmap(iconsize.width(),iconsize.height()));
    painter->drawPixmap(QPoint(iconRect.left()+iconsize.width()/2+2,iconRect.top()+iconsize.height()/2+3),icon.pixmap(iconsize.width(),iconsize.height()));

    painter->setFont(font);
    painter->drawText(headerRect,headerText);


    painter->setFont(SubFont);
    painter->drawText(subheaderRect.left(),subheaderRect.top(),subText);

    if(dateWidth) {
        painter->setFont(dateFont);
        painter->drawText(dateRect.left(), dateRect.top(), dateText);
    }

    painter->restore();

}
