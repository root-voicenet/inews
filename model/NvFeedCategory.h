#ifndef NVFEEDCATEGORY_H
#define NVFEEDCATEGORY_H

#include "NvAbstractTreeItem.h"

class NvFeedCategory : public NvAbstractTreeItem
{
    Q_OBJECT
public:
    NvFeedCategory(int id, const QString& title = "", NvFeedCategory *parent = 0);

    QList<int> feeds() const { return m_fids; }
    void addFeed(int fid);
    void removeFid(int fid);

    // overrided
    QVariant data(int role) const;
    static int maxID();
private:
    static int m_maxID;
    QList<int> m_fids;
};

#endif // NVFEEDCATEGORY_H
