#include "node.h"
#include "file.h"
#include "model/nvrssitem.h"
#include "resourcemanager.h"

Node::Node(int id, const QString& title, bool remote, bool created, const QString &body)
    : NvBaseObject(id, title, created), m_body(body),  m_updated(false), m_isremote(remote), promoted_(false)
{

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

void Node::setUpdated(bool updated)
{
    m_updated = updated;
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

    m_attached.append(file);
}

void Node::removeMedia(int id)
{
    for(int i = 0; i < m_attached.size(); ++i)
        if(m_attached[i].id() == id) {
            m_attached.removeAt(i);
            break;
        }
}

NvRssItem *Node::findAttachedRss(int id)
{
    QListIterator<NvRssItem*> i(m_attachedRss);
    while(i.hasNext()) {
        NvRssItem *item = i.next();
        if(item->id() == id)
            return item;
    }

    return NULL;
}

void Node::attachRss(NvRssItem *item)
{
    m_attachedRss.append(item);
}

void Node::removeAttachedRss(NvRssItem *item)
{
    m_attachedRss.removeOne(item);
}

QList<NvRssItem*> Node::attachedRss()
{
    return m_attachedRss;
}

QList<NvNodeMediaItem> &Node::attachedMedia()
{
    return m_attached;
}

NvNodeMediaItem &Node::findAttachedMedia(int id)
{
    for(int i = 0; i < m_attached.size(); ++i)
        if(m_attached[i].id() == id) {
            return m_attached[i];
        }
    return m_attached[0];
}
