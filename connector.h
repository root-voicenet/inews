#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <xmlrpc/client.h>
#include <QString>
#include <QMap>
#include "taxonomymodel.h"

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(const QString &url, QObject *parent = 0);
    ~Connector();

    void Login(const QString& username, const QString& password);
    void UploadFile(const QByteArray *postData, const QString &description, QList<int>& pointer_tids);
    void Sync();

    TaxonomyModel* getTaxonomy(int id) { return m_taxonomys[id]; }
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
    QMap<int, TaxonomyModel*> m_taxonomys;
    QMutex requestListMutex;

    // methods
    static const QString METHOD_SYSTEM_CONNECT;
    static const QString METHOD_USER_LOGOUT;
    static const QString METHOD_USER_LOGIN;
    static const QString METHOD_USER_CREATE;
    static const QString METHOD_TAXONOMY_GETTREE;
    static const QString METHOD_FILE_UPLOAD;
    static const QString METHOD_SYNC;


signals:
    /* Indicates user logged in and a session was received - ie successful log in */
    void logInFinished();

    /* Idicates that taxonomy tree is loaded */
    void taxonomyLoaded();

    /* Indicates photo upload request was sent and a reply received.
        For status check use isPhotoUploadSuccessful() */
    void fileUploadFinished();


    void loginNeeded();

    void syncComplete();

public slots:

    void processResponse(int id, QVariant responce);
    void failed( int requestId, int faultCode, QString faultString );
};

#endif // CONNECTOR_H
