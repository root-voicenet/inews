#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "feedmodel.h"
#include <QMutex>


class QNetworkAccessManager;
class QNetworkReply;

class ResourceManager : public QObject
{
    friend class Connector;

    Q_OBJECT
public:
    ResourceManager(QObject *parent = NULL);
    ~ResourceManager();
    FeedModel &getFeed() { return m_feed; }

    void DownloadIcon(const QString& url, QIcon *target);
private:
    typedef struct DownloadRequest {
        QIcon* icon;
    } DownloadRequest;

    FeedModel m_feed;
    QNetworkAccessManager* m_nam;

    //all requests go in this queue to be sent
    QVector<DownloadRequest> requestQueue;
    QMutex requestListMutex;
private slots:
    void finished(QNetworkReply* reply);
};

#endif // RESOURCEMANAGER_H
