#include "node.h"
#include "file.h"
#include "resourcemanager.h"
#include "newsapplication.h"

Node::Node(const QString& title, const QString& body)
    : m_title(title), m_body(body)
{

}

Node::~Node() {
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    for(int i = 0; i < m_attached.size(); ++i) {
        rm->removeFile(m_attached[i]);
    }
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
