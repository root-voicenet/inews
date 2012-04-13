#include "resourcemanager.h"
#include "node.h"
#include "file.h"
#include "rssitem.h"
#include "taxonomyterm.h"
#include "rsslistitemdelegate.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QImageReader>
#include <QDate>

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finished(QNetworkReply*)));

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::DownloadIcon(const QString &url, QStandardItem *target)
{
    QUrl durl(url);
    QNetworkReply* reply = m_nam->get(QNetworkRequest(durl));
    DownloadRequest d;
    d.item = target;
    requestQueue.push_back(d);

#ifdef DEBUG
    qDebug() << "new icon download request[" << url << "]";
#endif
}

void ResourceManager::finished(QNetworkReply *reply)
{

     requestListMutex.lock();
     if (requestQueue.size()<1){
        requestListMutex.unlock();
                qDebug()<<"network request has been deleted from queue before reply was received";
                return;
    }
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV =
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl =
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    // no error received?

    if(statusCodeV.toInt() == 200  && redirectionTargetUrl.toString().isEmpty()) {

        if (reply->error() == QNetworkReply::NoError)
        {
            // read data from QNetworkReply here
            QImageReader imageReader(reply);
            QPixmap pic;

           DownloadRequest d = requestQueue.first();
           if(d.item &&  pic.convertFromImage(imageReader.read())) {
               d.item->setIcon(pic);
           }
        }
        // Some http error received
        else
        {
            // handle errors here
        }
    }

    requestQueue.remove(0);
    requestListMutex.unlock();
    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    //delete reply;
}

void ResourceManager::clearTaxonomy(int id)
{
    if(id == TAXONOMY_THEME)
        qDeleteAll(m_themeTerms);
    else if(id == TAXONOMY_GEO)
        qDeleteAll(m_geoTerms);
}

bool ResourceManager::parseTaxonomy(int id, QVariant *resp)
{
    QList<QVariant> elements(resp->toList());

    clearTaxonomy(id);

    for (int i = 0; i < elements.size(); ++i) {
       // parse element
       QMap<QString, QVariant> tags = elements[i].toMap();
       QString name, depth;
       int tid;

       name = tags.value("name").toString();
       depth = tags.value("depth").toInt();
       tid = tags.value("tid").toInt();

       // build a parents
       QList<QVariant> parents = tags.value("parents").toList();
       for(int j = 0; j < parents.size(); ++j) {

       }

       TaxonomyTerm *term = new TaxonomyTerm(tid, name);
       if(id == TAXONOMY_THEME)
           m_themeTerms.append(term);
       else if(id == TAXONOMY_GEO)
           m_geoTerms.append(term);
   }
}

bool ResourceManager::parseFeed(QVariant *resp)
{
   QList<QVariant> elements(resp->toList());
   clearRssItems();


   for (int i = 0; i < elements.size(); ++i) {
       // parse element
       QMap<QString, QVariant> tags = elements[i].toMap();
       QString rssTitle;
       QString imageUrl;

       // create view
       int rssId;
       int cdate;

       rssTitle = tags.value("title").toString();
       rssId = tags.value("iid").toInt();
       cdate = tags.value("date").toInt();

       QList<QVariant> images = tags.value("image").toList();
       if(!images.isEmpty()) {
           imageUrl = images.first().toString();
       }



       if(rssTitle.isEmpty()) {
           qDebug() << "Title is empty";
           m_feed.clear();
           return false;
       }

       addRssItem(new RssItem(rssId, rssTitle, cdate, imageUrl));
   }
    return true;
}

void ResourceManager::cleanup()
{
    // clear rss items
    clearRssItems();
    // cleanup nodes
    clearNodes();

    // cleanup files
    qDeleteAll(m_files);

    // claer taxonomy
    clearTaxonomy(TAXONOMY_THEME);
    clearTaxonomy(TAXONOMY_GEO);
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

void ResourceManager::addRssItem(RssItem *item)
{
    QStandardItem *listitem = new QStandardItem();
    QString imageUrl = item->getImageUrl();
    QString prefix;

    if(!imageUrl.isEmpty()) {
       // Temporary disabled DownloadIcon(imageUrl, listitem);
    }

    QIcon icon(":/images/baloon.png");
    listitem->setData(item->getTitle(), RssListItemDelegate::HeaderTextRole);
    listitem->setData("Dummy", RssListItemDelegate::DescriptionRole);
    listitem->setData(icon, RssListItemDelegate::IconRole);

    QVariant data((int)item);
    listitem->setData(data);
    listitem->setEditable(false);

    int row = m_feed.rowCount();


    if(item->getCreated() > 0) {
        QDateTime date;
        date.setTime_t(item->getCreated());
        listitem->setData(date.toString(Qt::SystemLocaleShortDate), RssListItemDelegate::DateRole);
    }

    listitem->setEditable(false);
    m_feed.setItem(row, 0, listitem);

    m_rssitems.append(item);
}

void ResourceManager::removeRssItem(RssItem *item)
{
    int pos = m_rssitems.indexOf(item);
    if(pos != -1)
        m_rssitems.removeAt(pos);
}

void ResourceManager::clearRssItems()
{
    m_feed.clear();
    qDeleteAll(m_rssitems);
    m_rssitems.clear();
}

void ResourceManager::clearNodes()
{
    m_themes.clear();
    qDeleteAll(m_nodes);
    m_nodes.clear();
}

QList<TaxonomyTerm*> ResourceManager::getTaxonomy(int id)
{
    if(id == TAXONOMY_THEME)
        return m_themeTerms;
    else if(id == TAXONOMY_GEO)
        return m_geoTerms;
}

QList<RssItem*> ResourceManager::getUpdatedRss()
{
    QList<RssItem*> res;
    for(int i = 0; i < m_rssitems.size(); ++i) {
        if(!m_rssitems[i]->getTids().isEmpty())
            res.append(m_rssitems[i]);
    }

    return res;
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
    QListIterator<RssItem*> i(m_rssitems);
    while(i.hasNext()) {
        RssItem *n = i.next();

        if(n->getId() == id)
            return n;
    }

    return NULL;
}

Node *ResourceManager::parseNode(QVariant *resp)
{
    QMap<QString, QVariant> elements(resp->toMap());
    int nid = elements.value("node").toInt();
    Node *n = NULL;
    if(nid > 0 && (n = searchNode(nid)) != NULL) {
        n->setBody(elements.value("body").toString());
        return n;
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
