#include "node.h"
#include "file.h"
#include "model/nvrssitem.h"
#include "resourcemanager.h"

Node::Node(int id, const QString& title, bool remote, bool created, const QString &body)
    : NvBaseObject(id, title, created), m_body(body),  m_updated(false), m_isremote(remote)
{

}

Node::~Node() {
    ResourceManager *rm = ResourceManager::instance();
    for(int i = 0; i < m_attached.size(); ++i) {
        rm->removeFile(m_attached[i]);
    }
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

void Node::attachFile(File *file)
{
    if(m_attached.indexOf(file) != -1)
        return;

    ResourceManager *rm = ResourceManager::instance();
    if(!rm->lookupFile(*file)) {
        rm->addFile(*file);
    }
    m_attached.append(file);
}

void Node::removeFile(File *file)
{
    int pos = m_attached.indexOf(file);
    if(pos != -1) {
        m_attached.removeAt(pos);

        ResourceManager *rm = ResourceManager::instance();
        rm->removeFile(file);
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
