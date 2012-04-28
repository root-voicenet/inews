#ifndef NVRSSITEM_H
#define NVRSSITEM_H

#include "NvAbstractListItem.h"
#include <QPointer>
#include <QUrl>
#include <QList>

class QNetworkAccessManager;

class NvFeedItem;
class NvRssItem : public NvAbstractListItem
{
    QString name_, description_, source_;
    QIcon icon_;
    quint32 id_;
    QDateTime date_;
    QList<int> tids_;
    QString m_link;
    NvFeedItem *m_feed;
protected:
    bool updated_;
    bool promoted_;
public:
    NvRssItem(quint32 id, const QString & name = QString(), const QString & desc = QString());
    virtual ~NvRssItem(){}

    QString name() const;
    void setName(const QString &v);

    QString description() const;
    void setDescription(const QString &v);

    quint32 id() const;
    void setId(quint32 id);

    QIcon icon() const;
    void setIcon(const QIcon &v);

    QDateTime date() const;
    void setDate(const QDateTime &v);

    bool updated() const;
    void setUpdated(bool v);

    bool promoted() const;
    void setPromoted(bool v);


    QList<int> terms() const;
    void setTerms(const QList<int>& tids);

    QString link() const { return m_link; }
    void setLink(const QString &link);

    NvFeedItem *feed() const { return m_feed; }
    void setFeed( NvFeedItem *feed );

    QString termNames() const;
};

#endif // NVRSSITEM_H
