#include "NvFeedItem.h"

NvFeedItem::NvFeedItem(int id, const QString &title)
    :  NvAbstractTreeItem( id, title )
{
}

QVariant NvFeedItem::data(int role) const
{
    switch(role) {
        case Qt::DisplayRole:
            return m_name;
            break;
        default:
            return QVariant();
            break;
    }
}
