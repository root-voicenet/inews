#ifndef NODE_H
#define NODE_H

#include "nvbaseobject.h"
#include "model/NvNodeMediaItem.h"

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

    void attachMedia(const NvMediaItem &file, const QString& title = "", const QString& description = "");
    void removeMedia(int id);
    NvNodeMediaItem &findAttachedMedia(int id);
    QList<NvNodeMediaItem> &attachedMedia();

    void setBody(const QString& body);
    void setUpdated(bool updated);
    void setSummary(const QString& summary);

    bool promoted() const;
    void setPromoted(bool v);

    NvRssItem *findAttachedRss(int id);
    void attachRss(NvRssItem* item);
    void removeAttachedRss(NvRssItem *item);
    QList<NvRssItem *> attachedRss();
protected:
    bool m_isremote;
    bool m_updated;
    bool promoted_;
    QString m_body;
    QString m_summary;

    QList<NvNodeMediaItem> m_attached;
    QList<NvRssItem*> m_attachedRss;

};

#endif // NODE_H
