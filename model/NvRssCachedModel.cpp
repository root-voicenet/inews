#include "NvRssCachedModel.h"
#include "nvrssitem.h"
#include "NvFeedItem.h"
#include "../dbmanager.h"
#include <QDebug>

NvRssCachedModel::NvRssCachedModel(QObject *parent)
    : NvObjectModel(parent)
{
    m_storage = DBManager::instance();
    m_buffersize = 100;
    m_localCount = 0;

    updateLocalCount();
}



int NvRssCachedModel::localRssCount() const
{
    return m_count;
}

void NvRssCachedModel::clearRemote()
{

}

QVariant NvRssCachedModel::itemData(int row, int role) const
{
    if(row < 0 || row >= items.size())
        return QVariant();

    NvAbstractListItem * s = items[row];
    NvRssItem *item = dynamic_cast<NvRssItem*>( s );

    if(!item)
    {
            return QVariant();
    }

    switch(role)
    {
    case Qt::DecorationRole:
        return item->icon();
    case Qt::DisplayRole:
        return item->name();
    case DescriptionRole:
        return item->description();
    case FeedRole:
        if(item->feed())
            return item->feed()->name();
        break;
    case TagRole:
        return item->termNames();
        break;
    case FeedIdRole:
        if(item->feed())
            return item->feed()->id();
        break;
    case DateRole:
        return item->date();
    case PromotedRole:
        return item->promoted();
    }

    return QVariant();
}

bool NvRssCachedModel::storeRemote()
{
    /*
    if(items[m_remoteIdx].size() > 0) {
        for(int row = 0; row < items[m_remoteIdx].size(); ++row) {
            NvAbstractListItem *item = items[m_remoteIdx][row];

            if(dynamic_cast<NvRemoteRssItem*>(item)) {
                if(!m_storage->storeRss( dynamic_cast<NvRemoteRssItem*>(item) )) {
                    qDebug() << "can store item " << item->id();
                    continue;
                }

            }
        }
    } */

    return true;
}

void NvRssCachedModel::addRemote(NvRssItem *item)
{
    addItem(item);
}

void NvRssCachedModel::updateLocalCount()
{
    m_count = m_storage->rssCount();
}


bool NvRssCachedModel::canFetchMore(const QModelIndex &index) const
{
   int total = localRssCount();

   return m_localCount < total;
}


void NvRssCachedModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = localRssCount() - m_localCount;
    int itemsToFetch = qMin(m_buffersize, remainder);

    QList<NvRssItem*> l_items = m_storage->listRss(m_localCount, itemsToFetch);

    beginInsertRows(QModelIndex(), m_localCount, m_localCount + l_items.size() - 1);
    for(int j = 0; j < l_items.size(); ++j) {
        NvRssItem *item = l_items[j];

        item->setParent(this);
        connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));

        items << item;

        bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
        Q_ASSERT(ret);
    }
    m_localCount += l_items.size();
    endInsertRows();

}


QList<NvRssItem*> NvRssCachedModel::updatedRss() const
{
    QListIterator< NvAbstractListItemPtr > i(items);
    QList<NvRssItem*> items;
    while(i.hasNext()) {
        NvRssItem *item = dynamic_cast<NvRssItem*>(i.next().data());
        if(item->updated())
            items << item;
    }
    return items;
}
