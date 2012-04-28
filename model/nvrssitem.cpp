#include "nvrssitem.h"
#include "resourcemanager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

NvRssItem::NvRssItem(quint32 id, const QString &name, const QString &desc)
    : id_(id), name_(name), description_(desc)
{

}

QList<int> NvRssItem::terms() const
{
    return tids_;
}

void NvRssItem::setTerms(const QList<int> &tids)
{
    tids_ = tids;
}

void NvRssItem::setLink(const QString &link)
{
    m_link = link;
}

void NvRssItem::setFeed(NvFeedItem *feed)
{
    m_feed = feed;
}

quint32 NvRssItem::id() const {
    return id_;
}

void NvRssItem::setId(quint32 id)
{
    id_ = id;
}

QString NvRssItem::name() const
{
    return name_;
}

void NvRssItem::setName( const QString &v )
{
    name_ = v;
}

QString NvRssItem::description() const
{
    return description_;
}

void NvRssItem::setDescription( const QString &v )
{
    description_ = v;
}


QIcon NvRssItem::icon() const
{
    return icon_;
}

void NvRssItem::setIcon( const QIcon &v )
{
    icon_ = v;
}

QDateTime NvRssItem::date() const
{
    return date_;
}

void NvRssItem::setDate( const QDateTime &v )
{
    date_ = v;
}

bool NvRssItem::updated() const
{
    return updated_;
}

void NvRssItem::setUpdated( bool v )
{
    updated_ = v;
}

bool NvRssItem::promoted() const
{
    return promoted_;
}

void NvRssItem::setPromoted(bool v)
{
    promoted_ = v;
}

QString NvRssItem::termNames() const
{
    QString res;
    ResourceManager *rm = ResourceManager::instance();
    foreach(int tid, terms())
        res += rm->tag(tid) + " ";

    return res;
}
