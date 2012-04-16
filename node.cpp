#include "node.h"
#include "file.h"
#include "rssitem.h"
#include "resourcemanager.h"
#include "newsapplication.h"

Node::Node(int id, const QString& title, bool remote, bool created, const QString &body)
    : NvBaseObject(id, title, created), m_body(body),  m_updated(false), m_isremote(remote)
{

}

Node::~Node() {
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    for(int i = 0; i < m_attached.size(); ++i) {
        rm->removeFile(m_attached[i]);
    }
}

void Node::setBody(const QString &body)
{
    m_body = body;
}

void Node::setUpdated(bool updated)
{
    m_updated = updated;
}

void Node::attachFile(File *file)
{
    if(m_attached.indexOf(file) != -1)
        return;

    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
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

        ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
        rm->removeFile(file);
    }
}

RssItem* Node::findAttachedRss(int id)
{
    QListIterator<RssItem*> i(m_attachedRss);
    while(i.hasNext()) {
        RssItem *item = i.next();
        if(item->getId() == id)
            return item;
    }

    return NULL;
}

void Node::attachRss(RssItem *item)
{
    m_attachedRss.append(item);
}

void Node::removeAttachedRss(RssItem* item)
{
    m_attachedRss.removeOne(item);
}

QList<RssItem*> Node::attachedRss()
{
    return m_attachedRss;
}
