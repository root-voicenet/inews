#include "NvSortFilterModel.h"

NvSortFilterModel::NvSortFilterModel(QObject * parent):QSortFilterProxyModel(parent)
{
}

NvSortFilterModel::~NvSortFilterModel(void)
{
}

bool NvSortFilterModel::lessThan ( const QModelIndex & left, const QModelIndex & right ) const
{
	if(left.parent().isValid())
	{
		return QSortFilterProxyModel::lessThan(left, right);
	} else {
		return false;
	}
}

void NvSortFilterModel::install( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "install", Q_ARG(QModelIndex, srcIndex));
}

void NvSortFilterModel::remove( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "remove", Q_ARG(QModelIndex, srcIndex));
}

void NvSortFilterModel::updateItem( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "update", Q_ARG(QModelIndex, srcIndex));
}

void NvSortFilterModel::enable( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "enable", Q_ARG(QModelIndex, srcIndex));
}

void NvSortFilterModel::disable( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "disable", Q_ARG(QModelIndex, srcIndex));
}

void NvSortFilterModel::detail( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "detail", Q_ARG(QModelIndex, srcIndex));
}

void NvSortFilterModel::needUpdate_src( const QModelIndex & index )
{
	emit needUpdate(mapFromSource(index));
}

void NvSortFilterModel::setSourceModel( QAbstractItemModel * src )
{
	if(sourceModel())
	{
		disconnect(sourceModel(), SIGNAL(needUpdate(QModelIndex)),
			this, SLOT(needUpdate_src(QModelIndex)));
	}
	
	QSortFilterProxyModel::setSourceModel(src);

	connect(sourceModel(), SIGNAL(needUpdate(QModelIndex)),
		this, SLOT(needUpdate_src(QModelIndex)));
}

void NvSortFilterModel::cancel( const QModelIndex & index )
{
	QModelIndex srcIndex = mapToSource(index);
	QMetaObject::invokeMethod(sourceModel(), "cancel", Q_ARG(QModelIndex, srcIndex));
}
