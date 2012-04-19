#include "NvAbstractTreeItem.h"

NvAbstractTreeItem::NvAbstractTreeItem(NvAbstractTreeItem *parent)
{
    parentItem = parent;
}

NvAbstractTreeItem::~NvAbstractTreeItem()
{
    qDeleteAll(childItems);
}

void NvAbstractTreeItem::appendChild(NvAbstractTreeItem *child)
{
    child->parentItem = this;
    childItems.append(child);
}

NvAbstractTreeItem *NvAbstractTreeItem::child(int row) const
{
    return childItems.value(row);
}

int NvAbstractTreeItem::childCount() const
{
    return childItems.count();
}

NvAbstractTreeItem *NvAbstractTreeItem::parent()
{
    return parentItem;
}

int NvAbstractTreeItem::row() const
{
    if (parentItem)
         return parentItem->childItems.indexOf(const_cast<NvAbstractTreeItem*>(this));

         return 0;
}

void NvAbstractTreeItem::removeChild(NvAbstractTreeItem *child)
{
    int index = childItems.indexOf(child);
    if(index != -1) {
        childItems.removeAt(index);
        child->parentItem = NULL;
     }
}
