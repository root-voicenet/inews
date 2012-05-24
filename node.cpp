#include "node.h"
#include "file.h"
#include "model/nvrssitem.h"
#include "resourcemanager.h"

Node::Node(quint32 id, const QString& title, const QString &body)
    :_id(id), _title(title), m_body(body), promoted_(false), _status(STATUS_COMPOSED)
{

}

Node::Node(const Node &other)
{
    _id = other._id;
    _title = other._title;
    m_body = other.m_body;
    promoted_ = other.promoted_;
    _tags = other._tags;
    _created = other._created;
    m_attachedRss = other.m_attachedRss;
    m_attached = other.m_attached;
    _status = other._status;
}

Node::~Node() {

}

void Node::setBody(const QString &body)
{
    m_body = body;
}

void Node::setSummary(const QString &summary)
{
    m_summary = summary;
}

bool Node::promoted() const
{
    return promoted_;
}

void Node::setPromoted(bool v)
{
    promoted_ = v;
}

void Node::attachMedia(const NvMediaItem& file, const QString &title, const QString &description)
{
    for(int i = 0; i < m_attached.size(); ++i)
        if(m_attached[i].id() == file.id()) {
            return;
        }

    NvNodeMediaItem nv(file);
    nv.setTitle(title);
    nv.setDescription(description);
    m_attached.append(nv);
}

void Node::removeMedia(int id)
{
    for(int i = 0; i < m_attached.size(); ++i)
        if(m_attached[i].id() == id) {
            m_attached.removeAt(i);
            break;
        }
}

void Node::attachRss(quint32 rss_id)
{
    m_attachedRss.append(rss_id);
}

void Node::removeAttachedRss(quint32 rss_id)
{
    m_attachedRss.removeOne(rss_id);
}

QList<quint32> Node::attachedRss() const
{
    return m_attachedRss;
}

QList<NvNodeMediaItem> Node::attachedMedia() const
{
    return m_attached;
}

void Node::setAttached(const QList<NvNodeMediaItem> &items)
{
    m_attached = items;

}

NvNodeMediaItem &Node::findAttachedMedia(int id)
{
    for(int i = 0; i < m_attached.size(); ++i)
        if(m_attached[i].id() == id) {
            return m_attached[i];
        }
    return m_attached[0];
}
