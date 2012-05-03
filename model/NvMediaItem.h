#ifndef NVMEDIAITEM_H
#define NVMEDIAITEM_H

#include "NvAbstractListItem.h"

class QNetworkAccessManager;
class NvMediaItem : public NvAbstractListItem
{
    Q_OBJECT

 protected:
    QString filename_;
    quint32 id_;
    QIcon icon_;
    QString localPath_;
    bool is_remote;
    QString thumbnailUrl_;
    QNetworkAccessManager *manager_;

public:
    NvMediaItem(quint32 id, const QString & filename = QString(), bool isremote = false);
    NvMediaItem(const NvMediaItem& other);
    NvMediaItem& operator = (const NvMediaItem& item);


    quint32 id() const;
    void setId(quint32 id);
    QIcon icon() const;
    void setIcon(const QIcon &v);

    void setThumbnailUrl(const QString& url);

    QString name() const;

    QString localPath() const;
    void setLocalPath(const QString& filePath);

    void downloadThumbnail();
    void setNetworkAccessManager(QNetworkAccessManager *manager);
    QNetworkAccessManager *networkAccessManager();

    bool isRemote() const;
 private slots:
    void iconDownloaded();
};

#endif // NVMEDIAITEM_H
