#include "connector.h"
#include <QUrl>
#include <QProgressBar>
#include "newsapplication.h"
#include "resourcemanager.h"
#include "rssitem.h"
#include "node.h"
#include "taxonomyterm.h"
#include <qtextdocument.h>


 const QString Connector::METHOD_SYSTEM_CONNECT = "system.connect";
 const QString Connector::METHOD_USER_LOGOUT = "user.logout";
 const QString Connector::METHOD_USER_LOGIN = "user.login";
 const QString Connector::METHOD_USER_CREATE = "user.create";
 const QString Connector::METHOD_TAXONOMY_GETTREE = "node.taxonomy";
 const QString Connector::METHOD_FILE_UPLOAD = "media.upload";
 const QString Connector::METHOD_SYNC_RSS = "sync.rss";
 const QString Connector::METHOD_SYNC_NODES = "sync.nodes";
 const QString Connector::METHOD_NODE_GET = "node.full";
 const QString Connector::METHOD_NODE_CREATE = "node.create";

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

void Connector::CreateNode(Node *n)
{
    QMap<QString, xmlrpc::Variant> res;
    res.insert("title", n->getTitle());
    res.insert("body", n->getBody());

    int requestID = m_client.request(METHOD_NODE_CREATE, res);
    addRequest(requestID, METHOD_NODE_CREATE);
}

void Connector::SyncRss(QList<RssItem*> rss) {

    QList<xmlrpc::Variant> res;
    for(int i = 0; i < rss.size(); ++i) {
        QMap<QString, xmlrpc::Variant> resItem;
        resItem.insert("id", rss[i]->getId());

        QList<TaxonomyTerm*> tids = rss[i]->getTids();
        QList<xmlrpc::Variant> resTids;

        for(int j = 0; j < tids.size(); ++j)
            resTids.append(tids[j]->getId());

        resItem.insert("tids", resTids);

        res.append(resItem);
    }

    int requestID = m_client.request(METHOD_SYNC_RSS, res);
    addRequest(requestID, METHOD_SYNC_RSS);

    //int requestID = m_client.request(METHOD_TAXONOMY_GETTREE, 6);
    //addRequest(requestID, METHOD_TAXONOMY_GETTREE);
}

void Connector::SyncNodes(QList<Node *> nodes)
{
    QList<xmlrpc::Variant> res;
    for(int i = 0; i < nodes.size(); ++i) {
        QMap<QString, xmlrpc::Variant> nodeItem;
        QString action = "update";
        QList<xmlrpc::Variant> rss;
        if(nodes[i]->getId() == Node::NODEID_DEFAULT) {
            action = "create";
        }
        nodeItem.insert("action", action);
        nodeItem.insert("id", nodes[i]->getId());
        nodeItem.insert("title", nodes[i]->getTitle());

        if(!nodes[i]->getBody().isEmpty()) {
            nodeItem.insert("body", Qt::escape(nodes[i]->getBody()));
        }

        // attach tids
        QList<TaxonomyTerm*> tids = nodes[i]->getTids();
        QList<xmlrpc::Variant> nodeTids;

        for(int j = 0; j < tids.size(); ++j)
            nodeTids.append(tids[j]->getId());

        nodeItem.insert("tids", nodeTids);

        // attach rss ids
        QList<RssItem*> attached = nodes[i]->attachedRss();
        for(int j = 0; j < attached.size(); ++j)
            rss << attached[j]->getId();

        if(rss.size() > 0) {
            nodeItem.insert("rss", rss);
        }

        res.append(nodeItem);
    }

    int requestID = m_client.request(METHOD_SYNC_NODES, res);
    addRequest(requestID, METHOD_SYNC_NODES);
}

void Connector::GetNode(int id)
{
    int requestID = m_client.request(METHOD_NODE_GET, id);
    addRequest(requestID, METHOD_NODE_GET);
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
    QVariant param;
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
        if(it.value().param)
            param.setValue(*it.value().param);

        m_requests.remove(id);
    }

    requestListMutex.unlock();

    if(!method.isEmpty()) {

        if(method == METHOD_USER_LOGIN) {
            QMap<QString , QVariant> elements(responce.toMap());
            QString cookie = elements.value("session_name").toString();
            cookie.append("=");
            cookie.append(elements.value("sessid").toString());

            m_client.setCookie(cookie);
            m_isLogged = true;

            signal = &Connector::logInFinished;
        }else if(method == METHOD_TAXONOMY_GETTREE) {
            if(rm->parseTaxonomy(&responce)) {
                signal = &Connector::taxonomyLoaded;
            }
        }else if(method == METHOD_FILE_UPLOAD) {
            signal = &Connector::fileUploadFinished;
        }else if(method == METHOD_SYNC_RSS) {
           if(rm->parseFeed(&responce)) {
                signal = &Connector::syncRssComplete;
           }
        }else if(method == METHOD_SYNC_NODES) {
            if(rm->parseNodes(&responce)) {
                signal = &Connector::syncNodesComplete;
            }
        }else if(method == METHOD_NODE_GET) {
            Node *node = rm->parseNode(&responce);
            if(node) {
                emit Connector::nodeGetComplete( node );
            }
        }
    }

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
        requestID = m_client.request(METHOD_TAXONOMY_GETTREE);
        addRequest(requestID, METHOD_TAXONOMY_GETTREE);
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
    emit networkError(faultString);
}
