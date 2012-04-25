#include "NvSortFilterModel.h"
#include "NvObjectModel.h"

NvSortFilterModel::NvSortFilterModel(QObject * parent)
    : QSortFilterProxyModel(parent), m_fid(0)
{
}

NvSortFilterModel::~NvSortFilterModel(void)
{
}


void NvSortFilterModel::setSourceModel( QAbstractItemModel * src )
{
	QSortFilterProxyModel::setSourceModel(src);
}


void NvSortFilterModel::setFeedId(int id)
{
    m_fid = id;
    invalidateFilter();
}


bool NvSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if(!m_fid)
        return true;

    return qvariant_cast<int>(sourceModel()->data(index, NvObjectModel::FeedIdRole)) == m_fid;
}

