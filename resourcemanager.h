#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QMutex>
#include <QStandardItemModel>
#include "model/NvRssCachedModel.h"
#include "model/NvFeedModel.h"
#include "model/NvNodeModel.h"


class QNetworkAccessManager;
class QNetworkReply;
class Node;
class File;
class NvRssItem;
class TaxonomyTerm;
class Connector;

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

    ~ResourceManager();
    static ResourceManager* instance();
    void storeData();

    // manage rss items
    NvRssCachedModel *rssModel() { return &m_rssModel; }
    NvFeedModel *feedModel() { return &m_feedModel; }
    NvNodeModel *nodesModel() { return &m_nodes; }

    QDateTime getRssLastUpdateTime();

    TaxonomyTerm *searchTaxonomy(int id);
    QTreeWidgetItem *getTaxonomy();
    NvRssItem *searchRss(int id);
    QNetworkAccessManager *getNAM() { return m_nam; }

    QString tag(int id) const;
private:
    ResourceManager(QObject *parent = NULL);
    static ResourceManager* m_instance;
    QNetworkAccessManager *m_nam;

    // author content aka node
    NvNodeModel m_nodes;

    // and attached files
    QList<File*> m_files;
    QMap<int, QString> m_tags;

    NvRssCachedModel m_rssModel;
    NvFeedModel m_feedModel;

    // taxonomys
    QTreeWidgetItem* m_taxonomy;

    bool parseFeed(QVariant *resp);
    bool parseNodes(QVariant *resp);
    Node parseNode(QVariant *resp);
    bool parseTaxonomy(QVariant *resp);
    bool parseFeeds(QVariant *resp);
    bool parseEditFeed(QVariant *resp);
    bool parseMedia(QVariant *media);

    File *lookupFile(const File& file);
    void addFile(const File& file);
    void removeFile(File* file);

    void cleanup();
    void clearTaxonomy();
};

#endif // RESOURCEMANAGER_H
