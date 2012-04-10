#include "rssitem.h"

RssItem::RssItem(int id, const QString &title, const QString &imageUrl)
    : m_id(id), m_title(title), m_imageUrl(imageUrl)
{

}

void RssItem::setTids(const QList<int> &tids)
{
    m_tids = tids;
}

