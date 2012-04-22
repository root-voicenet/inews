#ifndef NVABSTRACTTREEITEM_H
#define NVABSTRACTTREEITEM_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QPointer>

class NvAbstractTreeItem : public QObject
{
    Q_OBJECT
public:
    explicit NvAbstractTreeItem(QObject *parent = 0);

    void appendChild(NvAbstractTreeItem *child);
    void removeChild(NvAbstractTreeItem *child);
    NvAbstractTreeItem *child(int row) const;

    int childCount() const;
    virtual QVariant data(int role) const = 0;
    virtual int row() const;
};

typedef QPointer< NvAbstractTreeItem > NvAbstractTreeItemPtr;

#endif // NVABSTRACTTREEITEM_H
