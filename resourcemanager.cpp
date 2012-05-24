#include "resourcemanager.h"
#include "node.h"
#include "file.h"
#include "model/nvrssitem.h"
#include "taxonomyterm.h"
#include "connector.h"
#include "loaders.h"
#include "model/NvRssCachedModel.h"
#include "model/NvMediaModel.h"
#include "model/Tag.h"
#include "windowmanager.h"
#include "mediawindow.h"
#include "dbmanager.h"

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
    return TagsManager::import( resp );
    /*
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

       m_tags.insert(ts.tid, ts.name);
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
    */
}

QString ResourceManager::tag(int id) const
{
    if(m_tags.contains(id)) {
        return m_tags[id];
    }
    return QString();
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
    return m_rssModel.import(resp);
}

void ResourceManager::cleanup()
{
    // clear rss items

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

QTreeWidgetItem* ResourceManager::getTaxonomy()
{
    return m_taxonomy;
}


bool ResourceManager::parseNodes(QVariant *resp)
{
    return m_nodes.import( resp );
}

bool ResourceManager::parseMedia(QVariant *media)
{
    QList<QVariant> elements(media->toList());
    for (int i = 0; i < elements.size(); ++i) {
        // parse element
        QMap<QString, QVariant> tags = elements[i].toMap();

    }

    return true;
}


NvRssItem *ResourceManager::searchRss(int id)
{
    //return dynamic_cast<NvRssItem*>(m_rssModel.find(id));
    return NULL;
}

Node ResourceManager::parseNode(QVariant *resp)
{


    QMap<QString, QVariant> elements(resp->toMap());
    MediaWindow *media = WindowManager::instance()->mediaWindow();
    NvMediaModel *model = MediaManager::mediaModel();


    int nid = elements.value("nid").toInt();
    Node n(0, "");
    if(nid > 0 && (n = m_nodes.search(nid)).id() > 0) {
        n.setBody(elements.value("body").toString());
        n.setSummary(elements.value("summary").toString());

        if(elements.contains("files")) {
            QList<QVariant> files = elements.value("files").toList();

            for(int i = 0; i < files.size(); ++i) {
                QMap<QString, QVariant> tags = files[i].toMap();
                //NvMediaItem *item = model->media(tags.value("fid").toInt());
                //if(item) {
                //    n.attachMedia(*item, tags.value("title").toString(), tags.value("description").toString());
                //}
            }
        }

        return n;
    }

    return Node(0, "");
}

void ResourceManager::storeData()
{
    //m_rssModel.storeRemote();
}

bool ResourceManager::parseFeeds(QVariant *resp)
{
    return m_feedModel.importFeeds(resp);
}

bool ResourceManager::parseEditFeed(QVariant *resp)
{
    return m_feedModel.importFeed(resp);
}


QDateTime ResourceManager::getRssLastUpdateTime()
{
    return DBManager::instance()->getUpdateTime(DBManager::TYPE_RSS);
}
