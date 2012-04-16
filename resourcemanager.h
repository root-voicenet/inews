#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "feedmodel.h"
#include <QMutex>
#include <QStandardItemModel>


class QNetworkAccessManager;
class QNetworkReply;
class Node;
class File;
class RssItem;
class TaxonomyTerm;

QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)

class ResourceManager : public QObject
{
    friend class Connector;
    friend class Node;

    Q_OBJECT
public:
    enum {
        TAXONOMY_THEME,
        TAXONOMY_GEO
    };

    ResourceManager(QObject *parent = NULL);
    ~ResourceManager();
    QStandardItemModel &getFeed() { return m_feed; }
    QStandardItemModel &getThemes() { return m_themes; }

    void DownloadIcon(const QString& url, QStandardItem *target);
    void addNode(Node *node);
    void removeNode(Node *node);
    void clearNodes();
    Node* searchNode(int id);
    TaxonomyTerm *searchTaxonomy(int id);

    QTreeWidgetItem *getTaxonomy();

    void addRssItem(RssItem *item);
    void removeRssItem(RssItem *item);
    void clearRssItems();
    RssItem *searchRss(int id);

    QList<RssItem*> getUpdatedRss();
    QList<Node*> getUpdatedNodes();
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

    // rss items
    QList<RssItem*> m_rssitems;

    // taxonomys
    QTreeWidgetItem* m_taxonomy;

    bool parseFeed(QVariant *resp);
    bool parseNodes(QVariant *resp);
    Node *parseNode(QVariant *resp);
    bool parseTaxonomy(QVariant *resp);

    File *lookupFile(const File& file);
    void addFile(const File& file);
    void removeFile(File* file);

    void cleanup();
    void clearTaxonomy();
private slots:
    void finished(QNetworkReply* reply);
};

#endif // RESOURCEMANAGER_H
