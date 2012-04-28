#ifndef NODE_H
#define NODE_H

#include "nvbaseobject.h"

class NvMediaItem;
class NvRssItem;

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
    QString getSummary() const { return m_summary; }

    bool isRemote() const { return m_isremote; }
    bool isUpdated() const { return m_updated; }

    void attachMedia(NvMediaItem* file);
    void removeMedia(NvMediaItem* file);
    void setBody(const QString& body);
    void setUpdated(bool updated);
    void setSummary(const QString& summary);

    NvRssItem *findAttachedRss(int id);
    void attachRss(NvRssItem* item);
    void removeAttachedRss(NvRssItem *item);
    QList<NvRssItem *> attachedRss();
protected:
    bool m_isremote;
    bool m_updated;
    QString m_body;
    QString m_summary;

    QList<NvMediaItem*> m_attached;
    QList<NvRssItem*> m_attachedRss;

};

#endif // NODE_H
