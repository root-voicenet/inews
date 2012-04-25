#include "nvrssitem.h"
#include "resourcemanager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

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

NvRssItem::NvRssItem(quint32 id, const QString& title)
    : NvBaseItem(id, title)
{
}


NvRemoteRssItem::NvRemoteRssItem(quint32 id, const QString& title)
    : NvRssItem(id, title)
{

}

QString NvRssItem::termNames() const
{
    QString res;
    ResourceManager *rm = ResourceManager::instance();
    foreach(int tid, terms())
        res += rm->tag(tid) + " ";
    return res;
}


void NvRemoteRssItem::setIconUrl(const QUrl &url)
{
    if( !url.isEmpty()) {
        QNetworkRequest req(url);
        QNetworkReply * repl = manager_->get(req);
        connect(repl, SIGNAL(finished()),
                this, SLOT(iconDownloaded()));
        connect(repl, SIGNAL(finished()),
                repl, SLOT(deleteLater()));
   }
}

void NvRemoteRssItem::iconDownloaded()
{
    QNetworkReply * repl = qobject_cast<QNetworkReply *>(sender());
    if( repl && repl->error() == QNetworkReply::NoError )
    {
            setIcon(QImage::fromData(repl->readAll()));
            repl->close();
            emit needUpdate();
    }
}

void NvRemoteRssItem::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    manager_ = manager;
}

QNetworkAccessManager *NvRemoteRssItem::networkAccessManager() const
{
    return manager_;
}

NvLocalRssItem::NvLocalRssItem(quint32 id, const QString &title)
    : NvRssItem(id, title)
{

}
