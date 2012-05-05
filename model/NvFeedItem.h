#ifndef NVFEEDITEM_H
#define NVFEEDITEM_H

#include "NvAbstractTreeItem.h"
#include <QString>


class NvFeedItem : public NvAbstractTreeItem
{
    Q_OBJECT
public:
    enum FeedIDS {
        NEW_FEED_ID = 99999
    };

    NvFeedItem(int id, const QString &title);
    virtual QVariant data(int role) const;
};

#endif // NVFEEDITEM_H
