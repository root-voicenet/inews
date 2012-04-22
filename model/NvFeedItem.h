#ifndef NVFEEDITEM_H
#define NVFEEDITEM_H

#include "NvAbstractTreeItem.h"
#include <QString>


class NvFeedItem : public NvAbstractTreeItem
{
    Q_OBJECT
public:
    NvFeedItem(int id, const QString& title = "");

    QString title() const { return m_title; }
    int id() { return m_id; }
    void setTitle(const QString& title);
    virtual QVariant data(int role) const;
private:
    QString m_title;
    int m_id;
};

#endif // NVFEEDITEM_H
