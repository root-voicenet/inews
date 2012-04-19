#include "NvFeedItem.h"

NvFeedItem::NvFeedItem(int id, const QString& title)
    :  m_id(id), m_title(title)
{
}

QVariant NvFeedItem::data(int role) const
{
    return QVariant();
}
