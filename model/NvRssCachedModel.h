#ifndef NVRSSCACHEDMODEL_H
#define NVRSSCACHEDMODEL_H

#include "NvObjectModel.h"

class DBManager;
class NvRemoteRssItem;
class NvRssItem;

class NvRssCachedModel : public NvObjectModel
{
public:
    NvRssCachedModel(QObject *parent = 0);
    bool store(const QModelIndex & index);
    void clearRemote();
    bool storeRemote();
    void addRemote( NvRemoteRssItem *item );
    QList<NvRssItem*> updatedRss() const;
private:
    int m_buffersize, m_count, m_localCount;
    DBManager *m_storage;

    int localRssCount() const;
    bool canFetchMore(const QModelIndex & index) const;
    void fetchMore(const QModelIndex & /* index */);
    void updateLocalCount();
};

#endif // NVRSSCACHEDMODEL_H
