#include "NvAbstractTreeItem.h"

NvAbstractTreeItem::NvAbstractTreeItem(QObject *parent)
    : QObject(parent)
{

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
