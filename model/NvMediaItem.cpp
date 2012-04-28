#include "NvMediaItem.h"
#include <QFileInfo>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

NvMediaItem::NvMediaItem(quint32 id, const QString &filename, bool isremote)
    : id_(id), filename_(filename), is_remote(isremote), manager_(NULL)
{
}

quint32 NvMediaItem::id() const {
    return id_;
}

void NvMediaItem::setId(quint32 id)
{
    id_ = id;
}

QString NvMediaItem::name() const
{
    return filename_;
}


QIcon NvMediaItem::icon() const
{
    return icon_;
}

void NvMediaItem::setIcon( const QIcon &v )
{
    icon_ = v;
}

QString NvMediaItem::localPath() const
{
    return localPath_;
}

void NvMediaItem::setLocalPath(const QString &filePath)
{
    localPath_ = filePath;
    is_remote = false;
}

void NvMediaItem::setThumbnailUrl(const QString &url)
{
    thumbnailUrl_ = url;
    is_remote = true;
}

void NvMediaItem::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    manager_ = manager;
}

QNetworkAccessManager *NvMediaItem::networkAccessManager()
{
    return manager_;
}

void NvMediaItem::downloadThumbnail()
{
    Q_ASSERT( manager_ );

    QUrl url(thumbnailUrl_);

    if( url.isValid() ) {
       QNetworkRequest req(url);
       QNetworkReply * repl = manager_->get(req);
       QObject::connect(repl, SIGNAL(finished()), this, SLOT(iconDownloaded()));
       QObject::connect(repl, SIGNAL(finished()), repl, SLOT(deleteLater()));
    }
}

void NvMediaItem::iconDownloaded()
{
    QNetworkReply * repl = qobject_cast<QNetworkReply *>(sender());
    if( repl && repl->error() == QNetworkReply::NoError )
    {
            setIcon(QPixmap::fromImage(QImage::fromData(repl->readAll())));
            repl->close();
            emit needUpdate();
    }
}

bool NvMediaItem::isRemote() const
{
    return is_remote;
}
