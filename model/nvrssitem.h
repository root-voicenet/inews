#ifndef NVRSSITEM_H
#define NVRSSITEM_H

#include "Tag.h"
#include <QPointer>
#include <QUrl>
#include <QList>

class QNetworkAccessManager;

class NvFeedItem;

class NvRssItem
{
    QString name_, description_, source_;
    quint32 id_;
    QDateTime date_;
    QList<Tag> tags_;
    QString m_link;
    NvFeedItem *m_feed;
    bool _readed;
protected:
    bool updated_;
public:
    NvRssItem();
    NvRssItem(quint32 id, const QString & name = QString(), const QString & desc = QString());
    NvRssItem(const NvRssItem& other);
    virtual ~NvRssItem(){}

    QString name() const;
    void setName(const QString &v);

    QString description() const;
    void setDescription(const QString &v);

    bool readed() const { return _readed; }
    void setReaded(bool readed) { _readed = readed; }

    QDateTime date() const;
    void setDate(const QDateTime &v);

    bool updated() const;
    void setUpdated(bool v);

    quint32 id() const;
    void setId(quint32 id);

    QList<Tag> tags() const;
    void setTags(const QList<Tag>& tags);

    QString link() const { return m_link; }
    void setLink(const QString &link);

    NvFeedItem *feed() const { return m_feed; }
    void setFeed( NvFeedItem *feed );

    QString termNames() const;
};

#endif // NVRSSITEM_H
