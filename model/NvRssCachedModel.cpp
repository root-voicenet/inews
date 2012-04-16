#include "NvRssCachedModel.h"
#include "dbmanager.h"
#include <QDebug>

NvRssCachedModel::NvRssCachedModel(QObject *parent)
    : NvObjectModel(parent)
{
    m_remoteIdx = addSection(tr("Remote"));
    m_loaclIdx = addSection(tr("Local"));

    m_storage = DBManager::instance();
    m_buffersize = 100;
    m_count = 0;
}



int NvRssCachedModel::localRssCount() const
{
    static int count =  m_storage->rssCount();

    return count;
}


bool NvRssCachedModel::canFetchMore(const QModelIndex &index) const
{

    if (index.isValid() && index.internalId() < (1 << mask()))
    {
        if(index.internalId() == m_loaclIdx) {
                int total = localRssCount();
                return items[index.internalId()].size() < total;
        }
     }

    return false;
}


NvAbstractListItem *NvRssCachedModel::getItem( const QModelIndex & index ) const
{
    NvAbstractListItem * item = 0;
    if(index.isValid() && index.parent().isValid()) {
                int section = index.parent().internalId();
                int row = index.row();

                if(section == m_loaclIdx && row >= items[section].size()) {
                     NvRssCachedModel* pthis = const_cast<NvRssCachedModel*>(this);
                     pthis->fetchMore(items[section].size(), row - items[section].size());
                }
                item = items[section][row];
    }
    return item;
}

/*
QVariant NvRssCachedModel::itemData(int section, int row, int role)
{
    if(items[section].size() <= row) {
        int local_count = items[section].size();
        int remainder = localRssCount() - local_count;
        int itemsToFetch = qMin(m_buffersize, remainder);

        QList<NvLocalRssItem*> l_items = m_storage->listRss(local_count, itemsToFetch);

        beginInsertRows(index(m_loaclIdx, 0), local_count, local_count + l_items.size());
        for(int j = 0; j < l_items.size(); ++j) {
            NvLocalRssItem *item = l_items[j];

            item->setParent(this);
            connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));

            items[m_loaclIdx] << item;

            bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
            Q_ASSERT(ret);
        }
        endInsertRows();
    }

    const NvAbstractListItem * item = items[section][row];

        if(!item)
        {
                return QVariant();
        }

        switch(role)
        {
        case DetailRole:
        //return item->detail();
        case Qt::DecorationRole:
                return item->icon();
        case Qt::DisplayRole:
                return item->name();
        case DescriptionRole:
                return item->description();
        case VersionRole:
        //return item->version();
        case InstalledRole:
        //return item->installed();
        case EnabledRole:
        //return item->enabled();
        case BuildInRole:
        //return item->buildIn();
        case SourceRole:
                return item->source();
        case DateRole:
                return item->date();
        case ProgressRole:
        //return item->progress();
        case GuidRole:
        //return item->guid();

        default:
                return QVariant();
        }
}
*/

void NvRssCachedModel::fetchMore(int start, int count)
{
    int remainder = localRssCount() - start;
    int itemsToFetch = qMin(qMax(m_buffersize, count), remainder);

    QList<NvLocalRssItem*> l_items = m_storage->listRss(start, itemsToFetch);

    beginInsertRows(index(m_loaclIdx, 0), start, start + l_items.size());
    for(int j = 0; j < l_items.size(); ++j) {
        NvLocalRssItem *item = l_items[j];

        item->setParent(this);
        connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));

        items[m_loaclIdx] << item;

        bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
        Q_ASSERT(ret);
    }
    endInsertRows();

}

int NvRssCachedModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
            if(parent.internalId() < (1 << magickNum()))
            {
                if(parent.internalId() == m_loaclIdx) {
                    return localRssCount();
                }
                return items[parent.internalId()].size();
            }


    } else {
            return items.size();
    }
    return 0;
}
