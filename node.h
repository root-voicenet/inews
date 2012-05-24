#ifndef NODE_H
#define NODE_H

#include "model/NvNodeMediaItem.h"
#include <QObject>
#include "model/Tag.h"


class Node
{
    friend class Connector;
public:
    enum {
        NODEID_DEFAULT = 0
    };

    enum NodeStatus {
        STATUS_COMPOSED,
        STATUS_OK
    };

    Node(quint32 id, const QString& title, const QString& body = "");
    Node(const Node& other);
    virtual ~Node();

    quint32 id() const { return _id; }
    void setId(quint32 id) { _id = id; }

    QString title() const { return _title; }
    void setTitle(const QString& title) { _title = title; }

    QString getBody() const { return m_body; }
    QString getSummary() const { return m_summary; }

    int status() const { return _status; }
    void setStatus(int status) { _status = status; }

    QDateTime date() const { return _created; }
    void setDate(const QDateTime& d) { _created = d; }

    QList<Tag> tags() const { return _tags; }
    void setTags(QList<Tag> tags) { _tags = tags; }

    void attachMedia(const NvMediaItem &file, const QString& title = "", const QString& description = "");
    void removeMedia(int id);
    void setAttached(const QList<NvNodeMediaItem>& items);
    NvNodeMediaItem &findAttachedMedia(int id);
    QList<NvNodeMediaItem> attachedMedia() const;

    void setBody(const QString& body);
    void setSummary(const QString& summary);

    bool promoted() const;
    void setPromoted(bool v);

    void attachRss(quint32 rss_id);
    void removeAttachedRss(quint32 rss_id);
    QList<quint32> attachedRss() const;
protected:
    bool promoted_;
    int _status;

    QString m_body;
    QString m_summary;
    QString _title;
    quint32 _id;
    QList<Tag> _tags;
    QDateTime _created;

    QList<NvNodeMediaItem> m_attached;
    QList<quint32> m_attachedRss;

};

#endif // NODE_H
