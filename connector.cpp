#include "connector.h"
#include <QUrl>
#include <QProgressBar>
#include "resourcemanager.h"
#include "node.h"
#include "taxonomyterm.h"
#include "requestbuilder.h"
#include "windowmanager.h"
#include "mediawindow.h"


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
const QString Connector::METHOD_RSS_FEEDS = "rss.feeds";
const QString Connector::METHOD_MEDIA_FILES = "media.files";

Connector::Connector(const QString& url, QObject *parent):
    Client(parent), m_isLogged(false)
{
    QUrl weburl(url);
    setHost(weburl.host(), 80, weburl.path());
    setUserAgent("Qt");

    connect(this, SIGNAL(done(int, QVariant)), this, SLOT(processResponse(int, QVariant)));
    connect(this, SIGNAL(failed(int, int, QString)), this, SLOT(failed(int, int, QString)));
}

Connector::~Connector()
{

}

void Connector::Login(const QString& username, const QString& password) {

    int requestID = request(METHOD_USER_LOGIN, username, password);
    addRequest(requestID, METHOD_USER_LOGIN);
}

void Connector::SyncRss() {

    xmlrpc::Variant req;
    ResourceManager *rm = ResourceManager::instance();

    if(RequestBuilder::buildSyncRss(&req, rm->rssModel())) {
        int requestID = request(METHOD_SYNC_RSS, req);
        addRequest(requestID, METHOD_SYNC_RSS);
    }
}

void Connector::SyncNodes()
{
    xmlrpc::Variant req;
    ResourceManager *rm = ResourceManager::instance();
    if(RequestBuilder::buildSyncNodes(&req, rm->nodesModel())) {
        int requestID = request(METHOD_SYNC_NODES, req);
        addRequest(requestID, METHOD_SYNC_NODES);
    }
}

void Connector::GetNode(int id)
{
    int requestID = request(METHOD_NODE_GET, id);
    addRequest(requestID, METHOD_NODE_GET);
}

void Connector::GetMedia()
{
    int requestID = request(METHOD_MEDIA_FILES);
    addRequest(requestID, METHOD_MEDIA_FILES);
}

void Connector::UploadFile(const QByteArray *postData, const QString &description, QList<int>& pointer_tids) {
    QByteArray buffer = postData->toBase64();
    QList<xmlrpc::Variant> tids;

    for(int i = 0; i < pointer_tids.size(); ++i)
        tids.append(pointer_tids[i]);

    QList<xmlrpc::Variant> param;
    param.append( buffer );
    param.append( description );
    param.append( tids );

    int requestID = request(METHOD_FILE_UPLOAD, param);
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
    ResourceManager *rm = ResourceManager::instance();

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

            setCookie(cookie);
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
        }else if(method == METHOD_RSS_FEEDS) {
            rm->parseFeeds(&responce);
            signal = &Connector::feedsLoaded;
        }else if(method == METHOD_MEDIA_FILES) {
            WindowManager::instance()->mediaWindow()->parseRemoteFiles(&responce);
            signal = &Connector::mediaLoaded;
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
        requestID = request(METHOD_TAXONOMY_GETTREE);
        addRequest(requestID, METHOD_TAXONOMY_GETTREE);

        requestID = request(METHOD_RSS_FEEDS);
        addRequest(requestID, METHOD_RSS_FEEDS);
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
