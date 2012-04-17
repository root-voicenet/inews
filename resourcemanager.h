#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QMutex>
#include <QStandardItemModel>
#include "model/NvRssCachedModel.h"


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

    ~ResourceManager();
    static ResourceManager* instance();
    void storeData();

    // manage rss items
    NvRssCachedModel *rssModel() { return &m_rssModel; }
    QStandardItemModel &getThemes() { return m_themes; }

    void addNode(Node *node);
    void removeNode(Node *node);
    void clearNodes();
    Node* searchNode(int id);
    TaxonomyTerm *searchTaxonomy(int id);

    QTreeWidgetItem *getTaxonomy();

    RssItem *searchRss(int id);

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
};

#endif // RESOURCEMANAGER_H
