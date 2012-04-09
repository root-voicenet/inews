#include "resourcemanager.h"
#include "node.h"
#include "file.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QImageReader>

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
}

void ResourceManager::finished(QNetworkReply *reply)
{
     requestListMutex.lock();
     if (requestQueue.size()<1){
        requestListMutex.unlock();
                qDebug()<<"network request has been deleted from queue before reply was received";
                QByteArray response(reply->readAll());
                QString resultString(response);
                qDebug()<<resultString;
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
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here
        QImageReader imageReader(reply);
        QPixmap pic = QPixmap();

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

    requestQueue.remove(0);
    requestListMutex.unlock();
    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    delete reply;
}

bool ResourceManager::parseFeed(QVariant *resp)
{
   QList<QVariant> elements(resp->toList());
   m_feed.clear();


   for (int i = 0; i < elements.size(); ++i) {
       // parse element
       QMap<QString, QVariant> tags = elements[i].toMap();
       QString rssTitle;
       QString imageUrl;
       int rssId;

       rssTitle = tags.value("title").toString();
       rssId = tags.value("iid").toInt();
       imageUrl = tags.value("image").toString();

       if(rssTitle.isEmpty()) {
           qDebug() << "Title is empty";
           m_feed.clear();
           return false;
       }

       QStandardItem *item = new QStandardItem(rssTitle);
       item->setCheckable( true );
       if(!imageUrl.isEmpty()) {
           DownloadIcon(imageUrl, item);

       }

       item->setData(rssId);
       m_feed.appendRow(item);
   }
    return true;
}

void ResourceManager::cleanup()
{
    // cleanup nodes
    qDeleteAll(m_nodes);

    // cleanup files
    qDeleteAll(m_files);
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
    m_themes.appendRow(newItem);
}

void ResourceManager::removeNode(Node *node)
{
    int pos = m_nodes.indexOf(node);
    if(pos != -1)
        m_nodes.removeAt(pos);
}
