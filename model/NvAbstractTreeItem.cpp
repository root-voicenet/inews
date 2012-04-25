#include "NvAbstractTreeItem.h"

NvAbstractTreeItem::NvAbstractTreeItem(int id, const QString &name, QObject *parent)
    : QObject(parent), m_id(id), m_name(name)
{

}

int NvAbstractTreeItem::id() const
{
    return m_id;
}

void NvAbstractTreeItem::setId( int id )
{
    m_id = id;
}

QString NvAbstractTreeItem::name() const
{
    return m_name;
}

void NvAbstractTreeItem::setName(const QString& name)
{
    m_name = name;
}

void NvAbstractTreeItem::appendChild(NvAbstractTreeItem *child)
{
    child->setParent(this);
}

NvAbstractTreeItem *NvAbstractTreeItem::child(int row) const
{
    return static_cast<NvAbstractTreeItem*>(children().at(row));
}

int NvAbstractTreeItem::childCount() const
{
    return children().count();
}


int NvAbstractTreeItem::row() const
{
    if (parent())
       return static_cast<NvAbstractTreeItem*>(parent())->children().indexOf(const_cast<NvAbstractTreeItem*>(this));

     return 0;
}

void NvAbstractTreeItem::removeChild(NvAbstractTreeItem *child)
{
    int index = children().indexOf(child);
    if(index != -1) {
        child->setParent(NULL);
        delete child;
     }
}
