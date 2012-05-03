#ifndef NVRSSCACHEDMODEL_H
#define NVRSSCACHEDMODEL_H

#include "NvObjectModel.h"

class DBManager;
class NvRssItem;

class NvRssCachedModel : public NvObjectModel
{
public:

    enum RssDataRole
    {
        FeedRole = Qt::UserRole + 100,
        DateRole,
        DescriptionRole,
        TagRole,
        FeedIdRole
    };

    NvRssCachedModel(QObject *parent = 0);
    bool store(const QModelIndex & index);
    void clearRemote();
    bool storeRemote();
    void addRemote( NvRssItem *item );
    QList<NvRssItem*> updatedRss() const;

protected:
    QVariant itemData(int row, int role ) const;

private:
    int m_buffersize, m_count, m_localCount;
    DBManager *m_storage;

    int localRssCount() const;
    bool canFetchMore(const QModelIndex & index) const;
    void fetchMore(const QModelIndex & /* index */);
    void updateLocalCount();
};

#endif // NVRSSCACHEDMODEL_H
