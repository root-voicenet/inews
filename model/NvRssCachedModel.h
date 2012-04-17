#ifndef NVRSSCACHEDMODEL_H
#define NVRSSCACHEDMODEL_H

#include "NvObjectModel.h"

class DBManager;
class NvRemoteRssItem;
class NvRssCachedModel : public NvObjectModel
{
public:
    NvRssCachedModel(QObject *parent = 0);
    bool store(const QModelIndex & index);
    void clearRemote();
    bool storeRemote();
    void addRemote( NvRemoteRssItem *item );
private:
    int m_remoteIdx, m_loaclIdx, m_buffersize, m_count;
    DBManager *m_storage;

    // overrided from NvObjectModel
    QVariant itemData(int section, int row, int role);
    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    NvAbstractListItem *getItem( const QModelIndex & index ) const;

    int localRssCount() const;
    bool canFetchMore(const QModelIndex & index) const;
    void fetchMore(int start, int count);
    void updateLocalCount();
};

#endif // NVRSSCACHEDMODEL_H
