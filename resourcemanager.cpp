#include "resourcemanager.h"
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

void ResourceManager::DownloadIcon(const QString &url, QIcon *target)
{
    QUrl durl(url);
    QNetworkReply* reply = m_nam->get(QNetworkRequest(durl));
    DownloadRequest d;
    d.icon = target;
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
       if(d.icon &&  pic.convertFromImage(imageReader.read())) {
           d.icon->addPixmap(pic);
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
