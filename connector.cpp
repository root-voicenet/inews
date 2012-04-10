#include "connector.h"
#include <QUrl>
#include <QProgressBar>
#include "newsapplication.h"
#include "resourcemanager.h"
#include "rssitem.h"



 const QString Connector::METHOD_SYSTEM_CONNECT = "system.connect";
 const QString Connector::METHOD_USER_LOGOUT = "user.logout";
 const QString Connector::METHOD_USER_LOGIN = "user.login";
 const QString Connector::METHOD_USER_CREATE = "user.create";
 const QString Connector::METHOD_TAXONOMY_GETTREE = "taxonomy_vocabulary.getTree";
 const QString Connector::METHOD_FILE_UPLOAD = "media.upload";
 const QString Connector::METHOD_SYNC_RSS = "sync.rss";


Connector::Connector(const QString& url, QObject *parent):
    QObject(parent), m_isLogged(false)
{
    QUrl weburl(url);
    m_client.setHost(weburl.host(), 80, weburl.path());
    m_client.setUserAgent("Qt");

    connect(&m_client, SIGNAL(done(int, QVariant)), this, SLOT(processResponse(int, QVariant)));
    connect(&m_client, SIGNAL(failed(int, int, QString)), this, SLOT(failed(int, int, QString)));
}

Connector::~Connector()
{

}

void Connector::Login(const QString& username, const QString& password) {

    int requestID = m_client.request(METHOD_USER_LOGIN, username, password);
    addRequest(requestID, METHOD_USER_LOGIN);

    //int requestID = m_client.request(METHOD_TAXONOMY_GETTREE, 6);
    //addRequest(requestID, METHOD_TAXONOMY_GETTREE);
}

void Connector::SyncRss(QList<RssItem*> rss) {

    QList<xmlrpc::Variant> res;
    for(int i = 0; i < rss.size(); ++i) {
        QMap<QString, xmlrpc::Variant> resItem;
        resItem.insert("id", rss[i]->getId());

        QList<int> tids = rss[i]->getTids();
        QList<xmlrpc::Variant> resTids;

        for(int j = 0; j < tids.size(); ++j)
            resTids.append(tids[j]);

        resItem.insert("tids", resTids);

        res.append(resItem);
    }

    int requestID = m_client.request(METHOD_SYNC_RSS, res);
    addRequest(requestID, METHOD_SYNC_RSS);

    //int requestID = m_client.request(METHOD_TAXONOMY_GETTREE, 6);
    //addRequest(requestID, METHOD_TAXONOMY_GETTREE);
}

void Connector::UploadFile(const QByteArray *postData, const QString &description, QList<int>& pointer_tids) {
    QByteArray buffer = postData->toBase64();
    QList<xmlrpc::Variant> tids;

    for(int i = 0; i < pointer_tids.size(); ++i)
        tids.append(pointer_tids[i]);

    int requestID = m_client.request(METHOD_FILE_UPLOAD, buffer, description, tids);
    addRequest(requestID, METHOD_FILE_UPLOAD);
}

void Connector::addRequest(int requestID, const QString& method, QVariant *param)
{
    requestListMutex.lock();
    Request r = { method, param };
    m_requests.insert(requestID, r);
    requestListMutex.unlock();
}

void Connector::processResponse(int id, QVariant responce)
{
    QMap<int, Request>::Iterator it;
    Connector::Signal signal = 0;
    QString method = "";
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();

    requestListMutex.lock();
    if(m_requests.size() < 1) {
        requestListMutex.unlock();
        qDebug() << "network request has been deleted from queue before reply was received";
        return;
    }

    it = m_requests.find(id);

    if(it != m_requests.end()) {
        method = it.value().method;

        if(method == METHOD_USER_LOGIN) {
            QMap<QString , QVariant> elements(responce.toMap());
            QString cookie = elements.value("session_name").toString();
            cookie.append("=");
            cookie.append(elements.value("sessid").toString());

            m_client.setCookie(cookie);
            m_isLogged = true;

            signal = &Connector::logInFinished;
        }else if(method == METHOD_TAXONOMY_GETTREE) {
            TaxonomyModel* m = new TaxonomyModel();
            int voc_id = (it.value().param) ? it.value().param->toInt() : 0;
            if(rm->parseTaxonomy(voc_id, &responce)) {
                signal = &Connector::taxonomyLoaded;
            }else
                delete m;
        }else if(method == METHOD_FILE_UPLOAD) {
            signal = &Connector::fileUploadFinished;
        }else if(method == METHOD_SYNC_RSS) {
           if(rm->parseFeed(&responce)) {
                signal = &Connector::syncComplete;
           }
        }


        m_requests.remove(id);
    }

    requestListMutex.unlock();

    // Process send post requests
    if(!method.isEmpty() && signal)
        sendPostRequest(method);

    if(signal != 0) {
        emit (this->*signal)();
    }
}

void Connector::sendPostRequest(const QString& method)
{
    int requestID;

    if(method == METHOD_USER_LOGIN) {
        requestID = m_client.request(METHOD_TAXONOMY_GETTREE, 13);
        addRequest(requestID, METHOD_TAXONOMY_GETTREE, new QVariant(ResourceManager::TAXONOMY_THEME));

        requestID = m_client.request(METHOD_TAXONOMY_GETTREE, 14);
        addRequest(requestID, METHOD_TAXONOMY_GETTREE, new QVariant(ResourceManager::TAXONOMY_GEO));
    }
}

void Connector::failed(int requestId, int faultCode, QString faultString)
{
    qDebug() << "failing request " << requestId << " error:" << faultString;
    QMap<int, Request>::Iterator it = m_requests.find(requestId);

    if(it != m_requests.end()) {
         m_requests.remove(requestId);
    }

    // need relogin
    emit loginNeeded();
}
