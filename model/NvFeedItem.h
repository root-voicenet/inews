#ifndef NVFEEDITEM_H
#define NVFEEDITEM_H

#include "NvAbstractTreeItem.h"
#include <QString>


class NvFeedItem : public NvAbstractTreeItem
{
    Q_OBJECT
public:
    NvFeedItem(int id, const QString &title);
    virtual QVariant data(int role) const;
};

#endif // NVFEEDITEM_H
