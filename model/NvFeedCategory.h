#ifndef NVFEEDCATEGORY_H
#define NVFEEDCATEGORY_H

#include "NvAbstractTreeItem.h"

class NvFeedCategory : public NvAbstractTreeItem
{
    Q_OBJECT
public:
    NvFeedCategory(int id, const QString& title = "", NvFeedCategory *parent = 0);

    QString title() const;
    int id() const;

    void setTitle( const QString& title );
    void setId( int id );
    QList<int> feeds() const { return m_fids; }
    void addFeed(int fid);
    void removeFid(int fid);

    // overrided
    QVariant data(int role) const;
    static int maxID();
private:
    static int m_maxID;
    QString m_title;
    int m_id;
    QList<int> m_fids;
};

#endif // NVFEEDCATEGORY_H
