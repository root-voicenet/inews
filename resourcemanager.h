#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QMutex>
#include <QStandardItemModel>
#include "model/NvRssCachedModel.h"
#include "model/NvFeedModel.h"


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
    QList<Node*> nodesModel() { return m_nodes; }
    QStandardItemModel& getThemes() { return m_themes; }

    void addNode(Node *node, bool top = false);
    void removeNode(Node *node);
    void clearNodes();
    Node* searchNode(int id);
    TaxonomyTerm *searchTaxonomy(int id);

    QTreeWidgetItem *getTaxonomy();

    NvRssItem *searchRss(int id);

    QList<Node*> getUpdatedNodes();

    QNetworkAccessManager *getNAM() { return m_nam; }
private:
    ResourceManager(QObject *parent = NULL);
    static ResourceManager* m_instance;
    QStandardItemModel m_themes;
    QNetworkAccessManager *m_nam;

    // author content aka node
    QList<Node*> m_nodes;

    // and attached files
    QList<File*> m_files;

    NvRssCachedModel m_rssModel;
    NvFeedModel m_feedModel;

    // taxonomys
    QTreeWidgetItem* m_taxonomy;

    bool parseFeed(QVariant *resp);
    bool parseNodes(QVariant *resp);
    Node *parseNode(QVariant *resp);
    bool parseTaxonomy(QVariant *resp);
    bool parseFeeds(QVariant *resp);

    File *lookupFile(const File& file);
    void addFile(const File& file);
    void removeFile(File* file);

    void cleanup();
    void clearTaxonomy();
};

#endif // RESOURCEMANAGER_H
