#ifndef NODE_H
#define NODE_H

#include "nvbaseobject.h"

class File;
class RssItem;

class Node : public NvBaseObject
{
    friend class Connector;
public:
    enum {
        NODEID_DEFAULT = 0
    };

    Node(int id, const QString& title, bool remote = false, bool created = 0, const QString& body = "");

    virtual ~Node();
    QString &getBody() { return m_body; }

    bool isRemote() const { return m_isremote; }
    bool isUpdated() const { return m_updated; }

    void attachFile(File* file);
    void removeFile(File* file);
    void setBody(const QString& body);
    void setUpdated(bool updated);

    RssItem *findAttachedRss(int id);
    void attachRss(RssItem* item);
    void removeAttachedRss(RssItem *item);
    QList<RssItem*> attachedRss();
protected:
    bool m_isremote;
    bool m_updated;
    QString m_body;

    QList<File*> m_attached;
    QList<RssItem*> m_attachedRss;

};

#endif // NODE_H
