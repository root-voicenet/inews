#include "rssitem.h"
#include "taxonomyterm.h"

RssItem::RssItem(int id, const QString &title, int created, const QString &imageUrl)
    : m_id(id), m_title(title), m_created(created), m_imageUrl(imageUrl), m_tids()
{

}

void RssItem::setTids(const QList<TaxonomyTerm*> &tids)
{
    m_tids = tids;
}

