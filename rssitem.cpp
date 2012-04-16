#include "rssitem.h"
#include "taxonomyterm.h"

RssItem::RssItem(int id, const QString &title, int created, const QString &imageUrl)
    : NvBaseObject(id, title, created), m_imageUrl(imageUrl)
{

}

void RssItem::setDescription(const QString &desc)
{
    m_description = desc;
}

void RssItem::setLink(const QString &link)
{
    m_link = link;
}
