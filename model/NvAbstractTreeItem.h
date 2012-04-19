#ifndef NVABSTRACTTREEITEM_H
#define NVABSTRACTTREEITEM_H

#include <QObject>
#include <QVariant>
#include <QList>

class NvAbstractTreeItem
{
public:
    explicit NvAbstractTreeItem(NvAbstractTreeItem *parent = 0);
    virtual ~NvAbstractTreeItem();

    void appendChild(NvAbstractTreeItem *child);
    void removeChild(NvAbstractTreeItem *child);
    NvAbstractTreeItem *child(int row) const;

    int childCount() const;
    virtual QVariant data(int role) const = 0;
    virtual int row() const;
    NvAbstractTreeItem *parent();

protected:
    QList<NvAbstractTreeItem*> childItems;
    NvAbstractTreeItem* parentItem;

};

#endif // NVABSTRACTTREEITEM_H
