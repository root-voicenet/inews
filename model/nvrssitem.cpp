#include "nvrssitem.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

NvRssItem::NvRssItem(quint32 id, const QString& title)
    : NvBaseItem(id, title)
{
}


NvRemoteRssItem::NvRemoteRssItem(quint32 id, const QString& title)
    : NvRssItem(id, title)
{

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
