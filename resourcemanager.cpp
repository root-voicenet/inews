#include "resourcemanager.h"
#include "node.h"
#include "file.h"
#include "rssitem.h"
#include "taxonomyterm.h"
#include "loaders.h"
#include "model/NvRssCachedModel.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QImageReader>
#include <QDate>
#include <QTreeWidgetItem>


ResourceManager *ResourceManager::m_instance = NULL;


class TermInternal {
public:
    int tid;
    int depth;
    QList<int> parents;
    QString name;
    QList<TermInternal> children;
    QTreeWidgetItem* treeItem;
};

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
{
    m_taxonomy = new QTreeWidgetItem();
    m_taxonomy->setText(0, "Root");

    m_nam = new QNetworkAccessManager(this);
    m_feedModel.init();
}

ResourceManager::~ResourceManager()
{

}

ResourceManager *ResourceManager::instance()
{
    if(!m_instance) {
        m_instance = new ResourceManager();
    }

    return m_instance;
}


void ResourceManager::clearTaxonomy()
{
    for(int i = 0; i < m_taxonomy->childCount(); ++i) {

    }
}

bool ResourceManager::parseTaxonomy(QVariant *resp)
{
    QList<QVariant> elements(resp->toList());
    QList<TermInternal> items;
    QMap<int, TermInternal*> refs;

    for (int i = 0; i < elements.size(); ++i) {
       // parse element
       QMap<QString, QVariant> tags = elements[i].toMap();
       TermInternal ts;

       ts.name = tags.value("name").toString();
       ts.depth = tags.value("depth").toInt();
       ts.tid = tags.value("tid").toInt();

       // build a parents
       QList<QVariant> parents = tags.value("parents").toList();
       for(int j = 0; j < parents.size(); ++j) {
           ts.parents << parents[j].toInt();
       }

       TaxonomyTerm *tx = new TaxonomyTerm(ts.tid, ts.name);

       QTreeWidgetItem *treeItem = new QTreeWidgetItem();
       treeItem->setData(0, Qt::UserRole + 1, (int)tx);
       treeItem->setText(0, ts.name);
       if(!ts.parents.isEmpty()) {
           treeItem->setCheckState(0, Qt::Unchecked);
       }
       ts.treeItem = treeItem;
       items << ts;
       refs[ts.tid] = &items.last();
   }

    QTreeWidgetItem *root = m_taxonomy;
    for(int i = 0; i < items.size(); ++i) {
        QTreeWidgetItem *croot = root;
        if(!items[i].parents.isEmpty() && refs.contains(items[i].parents.first())) {
            croot = refs[ items[i].parents.first() ]->treeItem;
        }

        croot->addChild( items[i].treeItem );
    }
}

TaxonomyTerm *ResourceManager::searchTaxonomy(int id)
{
    QVariant data;
    for(int i = 0; i < m_taxonomy->childCount(); ++i) {
        QTreeWidgetItem *item = m_taxonomy->child(i);

        if(item && !(data = item->data(0, Qt::UserRole + 1)).isNull()) {
            TaxonomyTerm* tax = reinterpret_cast<TaxonomyTerm*>(data.toInt());
            if(tax->getId() == id)
                return tax;
        }else if(item->childCount() > 0) {
            for(int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem *subitem = item->child(j);
                if(subitem && !(data = subitem->data(0, Qt::UserRole + 1)).isNull()) {
                    TaxonomyTerm* tax = reinterpret_cast<TaxonomyTerm*>(data.toInt());
                    if(tax->getId() == id)
                        return tax;
                }
            }
        }
    }

  return NULL;
}

bool ResourceManager::parseFeed(QVariant *resp)
{
    RssImporter importer(&m_rssModel);
    return importer.import(*resp);
}

void ResourceManager::cleanup()
{
    // clear rss items
    // cleanup nodes
    clearNodes();

    // cleanup files
    qDeleteAll(m_files);

    // claer taxonomy
    clearTaxonomy();
}

File *ResourceManager::lookupFile(const File& file)
{
    for(int i = 0; i < m_files.size(); ++i) {
        if(*(m_files[i]) == file)
            return m_files[i];
    }

    return NULL;
}

void ResourceManager::addFile(const File& file)
{
    m_files.append(new File(file));
}

void ResourceManager::removeFile(File *file)
{
    int pos = m_files.indexOf(file);
    if(pos != -1)
        m_files.removeAt(pos);
}

void ResourceManager::addNode(Node *node)
{
    m_nodes.append(node);

    QStandardItem *newItem = new QStandardItem(node->getTitle());
    newItem->setEditable(false);
    newItem->setData((int)node);
    m_themes.appendRow(newItem);
}

void ResourceManager::removeNode(Node *node)
{
    int pos = m_nodes.indexOf(node);
    if(pos != -1)
        m_nodes.removeAt(pos);
}

void ResourceManager::clearNodes()
{
    m_themes.clear();
    qDeleteAll(m_nodes);
    m_nodes.clear();
}

QTreeWidgetItem* ResourceManager::getTaxonomy()
{
    return m_taxonomy;
}


bool ResourceManager::parseNodes(QVariant *resp)
{
    QList<QVariant> elements(resp->toList());
    clearNodes();


    for (int i = 0; i < elements.size(); ++i) {
        // parse element
        QMap<QString, QVariant> tags = elements[i].toMap();
        QString nodeTitle;
        int nid = 0;


        nodeTitle = tags.value("title").toString();
        nid = tags.value("nid").toInt();
        if(!nid) {
            qDebug() << "Node Id is empty";
            clearNodes();
            return false;
        }

        if(nodeTitle.isEmpty()) {
            qDebug() << "Node Title is empty";
            clearNodes();
            return false;
        }

        Node *node = new Node(nid, nodeTitle, true);

        if(!tags.value("rss").isNull()) {
            QList<QVariant> rss = tags.value("rss").toList();
            QListIterator<QVariant> i(rss);
            while(i.hasNext()) {
                int rss_id = i.next().toInt();
                RssItem *rssItem;

                if(rss_id && (rssItem = searchRss(rss_id)) != NULL) {
                    node->attachRss(rssItem);
                }
            }
        }

        addNode(node);
    }

    return true;
}

Node *ResourceManager::searchNode(int id)
{
    QListIterator<Node*> i(m_nodes);
    while(i.hasNext()) {
        Node *n = i.next();

        if(n->getId() == id)
            return n;
    }

    return NULL;
}

RssItem *ResourceManager::searchRss(int id)
{
    /*
    QListIterator<RssItem*> i(m_rssitems);
    while(i.hasNext()) {
        RssItem *n = i.next();

        if(n->getId() == id)
            return n;
    }
    */

    return NULL;
}

Node *ResourceManager::parseNode(QVariant *resp)
{
    QMap<QString, QVariant> elements(resp->toMap());

    int nid = elements.value("nid").toInt();
    Node *n = NULL;
    if(nid > 0 && (n = searchNode(nid)) != NULL) {
        n->setBody(elements.value("body").toString());
        n->setSummary(elements.value("summary").toString());
    }

    return NULL;
}

QList<Node*> ResourceManager::getUpdatedNodes()
{
    QList<Node*> res;
    for(int i = 0; i < m_nodes.size(); ++i) {
        if(m_nodes[i]->isUpdated())
            res.append(m_nodes[i]);
    }

    return res;
}

void ResourceManager::storeData()
{
    m_rssModel.storeRemote();
}
