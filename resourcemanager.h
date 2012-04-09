#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "feedmodel.h"
#include <QMutex>


class QNetworkAccessManager;
class QNetworkReply;
class Node;
class File;

class ResourceManager : public QObject
{
    friend class Connector;
    friend class Node;

    Q_OBJECT
public:
    ResourceManager(QObject *parent = NULL);
    ~ResourceManager();
    QStandardItemModel &getFeed() { return m_feed; }
    QStandardItemModel &getThemes() { return m_themes; }

    void DownloadIcon(const QString& url, QStandardItem *target);
    void addNode(Node *node);
    void removeNode(Node *node);
private:
    typedef struct DownloadRequest {
        QStandardItem* item;
    } DownloadRequest;

    QStandardItemModel m_feed, m_themes;
    QNetworkAccessManager* m_nam;

    //all requests go in this queue to be sent
    QVector<DownloadRequest> requestQueue;
    QMutex requestListMutex;

    // author content aka node
    QList<Node*> m_nodes;

    // and attached files
    QList<File*> m_files;

    bool parseFeed(QVariant *resp);
    File *lookupFile(const File& file);
    void addFile(const File& file);
    void removeFile(File* file);

    void cleanup();
private slots:
    void finished(QNetworkReply* reply);
};

#endif // RESOURCEMANAGER_H
