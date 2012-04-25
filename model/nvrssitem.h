#ifndef NVRSSITEM_H
#define NVRSSITEM_H

#include "NvBaseItem.h"
#include <QPointer>
#include <QUrl>
#include <QList>

class QNetworkAccessManager;

class NvFeedItem;
class NvRssItem : public NvBaseItem
{
public:
    NvRssItem(quint32 id, const QString& title);

    QList<int> terms() const;
    void setTerms(const QList<int>& tids);
    QString link() const { return m_link; }
    NvFeedItem *feed() const { return m_feed; }
    QString termNames() const;

    void setLink(const QString &link);
    void setFeed( NvFeedItem *feed );
protected:
    QList<int> tids_;
    QString m_link;
    NvFeedItem *m_feed;
};

class NvRemoteRssItem : public NvRssItem
{
    Q_OBJECT
public:
    NvRemoteRssItem(quint32 id, const QString& title);

    void setIconUrl(const QUrl & url);

    void setNetworkAccessManager(QNetworkAccessManager * manager);
    QNetworkAccessManager *networkAccessManager() const;

private:
    QPointer<QNetworkAccessManager> manager_;

private slots:
        void iconDownloaded();
};

class NvLocalRssItem : public NvRssItem
{
public:
    NvLocalRssItem(quint32 id, const QString& title);
};

#endif // NVRSSITEM_H
