#include "nvrssitem.h"
#include "resourcemanager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

NvRssItem::NvRssItem()
    : id_(0), name_(""), description_(""), updated_(false), _readed(false)
{

}

NvRssItem::NvRssItem(quint32 id, const QString &name, const QString &desc)
    : id_(id), name_(name), description_(desc), updated_(false), _readed(false)
{

}

NvRssItem::NvRssItem(const NvRssItem &other)
{
    id_ = other.id_;
    name_ = other.name_;
    description_ = other.description_;
    date_ = other.date_;
    tags_ = other.tags_;
    m_feed = other.m_feed;
    m_link = other.m_link;
    _readed = other._readed;
}

QList<Tag> NvRssItem::tags() const
{
    return tags_;
}

void NvRssItem::setTags(const QList<Tag> &tags)
{
    tags_ = tags;
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

QString NvRssItem::termNames() const
{
    QString res;

    foreach(Tag tag, tags())
        res += tag.name() + " ";

    return res;
}
