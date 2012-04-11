#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QList>

class File;
class RssItem;

class Node
{
    friend class Connector;
public:
    enum {
        NODEID_DEFAULT = 0
    };

    Node(int id, const QString& title, bool remote = false, const QString& body = "");
    virtual ~Node();
    QString getTitle() const { return m_title; }
    QString &getBody() { return m_body; }
    int getId() const { return m_id; }
    bool isRemote() const { return m_isremote; }
    bool isUpdated() const { return m_updated; }

    void attachFile(File* file);
    void removeFile(File* file);
    void setTitle(const QString& title);
    void setBody(const QString& body);
    void setUpdated(bool updated);


    RssItem *findAttachedRss(int id);
    void attachRss(RssItem* item);
    void removeAttachedRss(RssItem *item);
    QList<RssItem*> attachedRss();
protected:
    int m_id;
    bool m_isremote;
    bool m_updated;
    QString m_title;
    QString m_body;
    QList<File*> m_attached;
    QList<RssItem*> m_attachedRss;

};

#endif // NODE_H
