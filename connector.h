#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <xmlrpc/client.h>
#include <QString>
#include <QMap>

class RssItem;
class Node;

QT_FORWARD_DECLARE_CLASS(QProgressBar)

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(const QString &url, QObject *parent = 0);
    ~Connector();

    void Login(const QString& username, const QString& password);
    void UploadFile(const QByteArray *postData, const QString &description, QList<int>& pointer_tids);
    void SyncRss();
    void SyncNodes();
    void GetNode(int id);

    // Remove later
    void CreateNode(Node *n);
private:
    //To allow to return pointers to signals
    typedef void (Connector::*Signal)();
    typedef struct _Request {
        QString method;
        QVariant *param;
    } Request;

    void connectService();
    void addRequest(int requestID, const QString& method, QVariant *param = NULL);
    void sendPostRequest(const QString& method);

    xmlrpc::Client m_client;
    bool m_isLogged;
    bool m_isConnected;
    QMap<int, Request> m_requests;
    QMutex requestListMutex;

    // methods
    static const QString METHOD_SYSTEM_CONNECT;
    static const QString METHOD_USER_LOGOUT;
    static const QString METHOD_USER_LOGIN;
    static const QString METHOD_USER_CREATE;
    static const QString METHOD_TAXONOMY_GETTREE;
    static const QString METHOD_FILE_UPLOAD;
    static const QString METHOD_SYNC_RSS;
    static const QString METHOD_SYNC_NODES;
    static const QString METHOD_NODE_GET;

    //TODO: Remove later
    static const QString METHOD_NODE_CREATE;

signals:
    /* Indicates user logged in and a session was received - ie successful log in */
    void logInFinished();

    /* Idicates that taxonomy tree is loaded */
    void taxonomyLoaded();

    /* Indicates photo upload request was sent and a reply received.
        For status check use isPhotoUploadSuccessful() */
    void fileUploadFinished();


    void networkError(QString);

    void syncRssComplete();
    void syncNodesComplete();

    void nodeGetComplete(Node *node);
public slots:

    void processResponse(int id, QVariant responce);
    void failed( int requestId, int faultCode, QString faultString );
};

#endif // CONNECTOR_H
