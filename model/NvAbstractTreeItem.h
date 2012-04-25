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
    explicit NvAbstractTreeItem(int id = 0, const QString& name = QString(), QObject *parent = 0);

    void appendChild(NvAbstractTreeItem *child);
    void removeChild(NvAbstractTreeItem *child);
    NvAbstractTreeItem *child(int row) const;

    int childCount() const;

    int id() const;
    QString name() const;
    void setId(int id);
    void setName(const QString& name);

    virtual QVariant data(int role) const = 0;
    virtual int row() const;

protected:
    int m_id;
    QString m_name;
};

typedef QPointer< NvAbstractTreeItem > NvAbstractTreeItemPtr;

#endif // NVABSTRACTTREEITEM_H
