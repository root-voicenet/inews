#include "NvFeedItem.h"

NvFeedItem::NvFeedItem(int id, const QString& title)
    :  m_id(id), m_title(title)
{
}

QVariant NvFeedItem::data(int role) const
{
    switch(role) {
        case Qt::DisplayRole:
            return m_title;
            break;
        default:
            return QVariant();
            break;
    }
}

void NvFeedItem::setTitle(const QString &title)
{
    m_title = title;
}
