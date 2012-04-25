#include "NvObjectModel.h"
#include "nvrssitem.h"
#include <QCoreApplication>
#include <QDebug>
#include "NvFeedItem.h"
#include "resourcemanager.h"

NvObjectModel::NvObjectModel(QObject *parent)
        : QAbstractListModel(parent)
{
}

NvObjectModel::~NvObjectModel()
{
        ItemVector::iterator i;

	
	items.clear();
}

int NvObjectModel::rowCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
        return items.size();
}


int NvObjectModel::columnCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	return 1;
}

QVariant NvObjectModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
        if (index.isValid())
	{
            return itemData( index.row(), role);
	}
	return QVariant();
}

QVariant NvObjectModel::itemData(int row, int role ) const
{
    if(row < 0 || row >= items.size())
        return QVariant();

    NvAbstractListItem * s = items[row];
    NvRssItem *item = dynamic_cast<NvRssItem*>(s);

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
	case BuildInRole:
        //return item->buildIn();
	case SourceRole:
		return item->source();
	case DateRole:
		return item->date();
    case PromotedRole:
        return item->promoted();
	}

    return QVariant();
}

NvAbstractListItem *NvObjectModel::item(const QModelIndex &index) const
{
    return getItem( index );
}


bool NvObjectModel::addItem( NvAbstractListItem * item )
{
	if (item)
	{
		item->setParent(this);
		connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));
                beginInsertRows(index(0, 0), items.count(), items.count());
                items << item;

		bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
		Q_ASSERT(ret);
	
		endInsertRows();
		return true;
	} else {
		return false;
	}
}


Qt::ItemFlags NvObjectModel::flags( const QModelIndex & index ) const
{
    NvAbstractListItem * item = getItem(index);
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
	if(item)
	{
		f |= item->flags();
	}
	return f;
}

NvAbstractListItem *NvObjectModel::getItem( const QModelIndex & index ) const
{
    NvAbstractListItem * item = 0;
    if(index.isValid() && index.row() < items.size()) {
        int row = index.row();

        item = items[row];
    }
    return item;
}

void NvObjectModel::itemNeedUpdate()
{
    NvAbstractListItem * item = dynamic_cast<NvAbstractListItem*>(sender());
        int row = 0;
        row = items.indexOf(item);
        if(row != -1)
	{
		qDebug() << "Need update item";
                emit needUpdate(index(row, 0));
	}
}

void NvObjectModel::itemDeleted( QObject * obj )
{
    NvAbstractListItem * item = qobject_cast<NvAbstractListItem*>(obj);
     int row = 0;
    if( (row = items.indexOf(item)) != -1 )
    {
            beginRemoveRows(index(0, 0), row, row);
            items.removeAll(item);
//qDebug() << "Remove item: " << item->name();
            endRemoveRows();
    }
}

bool NvObjectModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole */ )
{

    NvAbstractListItem * item = items[index.row()];

    if(!item)
    {
            return QAbstractItemModel::setData(index, value, role);
    }

    switch(role)
    {
    case DetailRole:
    //item->setDetail(value.toString()); break;
    case DescriptionRole:
            item->setDescription(value.toString()); break;
    case SourceRole:
            item->setSource(value.toString()); break;
    case DateRole:
            item->setDate(value.toDateTime()); break;
    case NeedUpdateRole:
            item->setUpdated(value.toBool()); break;
    default:
            return QAbstractItemModel::setData(index, value, role);
    }
	return true;
}

NvAbstractListItem *NvObjectModel::find(int id) const
{
    QListIterator< NvAbstractListItemPtr > i(items);

    while(i.hasNext()) {
        NvAbstractListItem *item = i.next();
        if(item->id() == id)
            return item;
    }
    return NULL;
}
