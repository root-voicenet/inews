#include "NvObjectModel.h"
#include "NvAbstractListItem.h"
#include <QCoreApplication>
#include <QDebug>

NvObjectModel::NvObjectModel(QObject *parent)
	: QAbstractItemModel(parent)
{
}

NvObjectModel::~NvObjectModel()
{
	QVector< ItemVector >::iterator i;

	
	items.clear();
}

int NvObjectModel::rowCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	if (parent.isValid())
	{
		if(parent.internalId() < (1 << magickNum()))
		{
			return items[parent.internalId()].size();
		}
	} else {
		return items.size();
	}
	return 0;
}

QModelIndex NvObjectModel::parent( const QModelIndex & index ) const
{
	if(index.internalId() >= (1 << magickNum()) )
	{
		return createIndex(index.row(), index.column(), int(index.internalId() & mask()));
	}
	return QModelIndex();

}

int NvObjectModel::columnCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	return 1;
}

QVariant NvObjectModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
	if (index.isValid())
	{
		if(index.parent().isValid())
                {
                    NvObjectModel *pthis = const_cast<NvObjectModel*>(this);
                        return pthis->itemData( index.internalId() & mask(), index.row(), role);
		} else {
			// Заголовок разделов
			switch(role)
			{
			case Qt::DisplayRole:
				return headers_[index.row()];
			case DetailRole:
				{
                                        int itemsCount_ = rowCount(index);//,
                                        //	itemsUCount_ = rowWUpdateCount(index);
					QStringList items_;
					items_ << QCoreApplication::translate("update", "%1 items", "total count of items", QCoreApplication::CodecForTr, itemsCount_ ).arg(itemsCount_);
                                        //if(itemsUCount_ > 0)
                                        //	items_ << QCoreApplication::translate("update", "(need update %1)", "total count of items", QCoreApplication::CodecForTr, itemsUCount_ ).arg(itemsUCount_);
					return items_.join(" ");
				}
			}
		}
	}
	return QVariant();
}

QVariant NvObjectModel::itemData(int section, int row, int role )
{
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

NvAbstractListItem *NvObjectModel::item(const QModelIndex &index) const
{
    return getItem( index );
}

QModelIndex NvObjectModel::index( int row, int column, const QModelIndex & parent /*= QModelIndex() */ ) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	int id = -1;
	if( parent.isValid() )
	{
		id = (row + 1) << magickNum();
		id = id | parent.internalId();		
	} else {
		id = row;
	}
	if(id != -1)
		return createIndex(row, column, id);
	else
		return QModelIndex();
}

int NvObjectModel::addSection( const QString & text )
{
	//beginInsertRows(QModelIndex(), items.count(), items.count());
	items << ItemVector();
	headers_ << text;
	int newRow = items.size() -1;
	beginInsertRows(QModelIndex(), newRow, newRow);	
	endInsertRows();
	return newRow;
}

bool NvObjectModel::addInSection( int section, NvAbstractListItem * item )
{
	if (item)
	{
		item->setParent(this);
		connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));
		beginInsertRows(index(section, 0), items[section].count(), items[section].count());
		items[section] << item;

		bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
		Q_ASSERT(ret);
	
		endInsertRows();
		return true;
	} else {
		return false;
	}
}

void NvObjectModel::removeSection( int section )
{
	if(section < items.size())
	{
        clearSection( section );

		beginRemoveRows(QModelIndex(), section, section);
        items.remove(section);
		headers_.removeAt(section);
		endRemoveRows();
	}
}

void NvObjectModel::clearSection(int section)
{
    if(section < items.size())
    {
        beginRemoveRows(QModelIndex(), section, section);
        qDeleteAll(items[section]);
        endRemoveRows();
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

void NvObjectModel::install( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "install");
	}
}

void NvObjectModel::remove( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "remove");
	}
}

void NvObjectModel::updateItem( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "update");
	}
}

void NvObjectModel::enable( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "enable");
	}
}

void NvObjectModel::disable( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "disable");
	}
}

void NvObjectModel::detail( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "detail");
	}
}

NvAbstractListItem *NvObjectModel::getItem( const QModelIndex & index ) const
{
    NvAbstractListItem * item = 0;
    if(index.isValid() && index.parent().isValid()) {
		int section = index.parent().internalId();
		int row = index.row();

		item = items[section][row];
    }
    return item;
}

void NvObjectModel::itemNeedUpdate()
{
    NvAbstractListItem * item = dynamic_cast<NvAbstractListItem*>(sender());
	int section = 0, row = 0;
	for (int section = 0; section < items.count(); ++section)
	{
		row = items[section].indexOf(item);
		if(row != -1)
			break;
	}
	if(section != items.count() && row != -1)
	{
		QModelIndex parent = index(section, 0, QModelIndex());
		qDebug() << "Need update item";
		emit needUpdate(index(row, 0, parent));
	}
}

void NvObjectModel::cancel( const QModelIndex & index )
{
	QObject * obj = dynamic_cast<QObject*>(getItem(index));
	if(obj)
	{
		QMetaObject::invokeMethod(obj, "cancel");
	}
}

void NvObjectModel::itemDeleted( QObject * obj )
{
    NvAbstractListItem * item = qobject_cast<NvAbstractListItem*>(obj);
	int section = 0, row = 0;
	for (int section = 0; section < items.count(); ++section)
	{
		if( (row = items[section].indexOf(item)) != -1 )
		{
			beginRemoveRows(index(section, 0), row, row);
			items[section].removeAll(item);
            //qDebug() << "Remove item: " << item->name();
			endRemoveRows();
		}		
	}
}

bool NvObjectModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole */ )
{

	if ( !(index.isValid() && index.parent().isValid()) )
	{
		// Заголовок разделов
		return QAbstractItemModel::setData(index, value, role);
	}

    NvAbstractListItem * item = items[index.internalId() & mask()][index.row()];

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
	case VersionRole:
        //item->setVersion(value.toInt()); break;
	case SourceRole:
		item->setSource(value.toString()); break;
	case DateRole:
		item->setDate(value.toDate()); break;
	case NeedUpdateRole:
		item->setHasUpdate(value.toBool()); break;
	default:
		return QAbstractItemModel::setData(index, value, role);
	}
	return true;
}

int NvObjectModel::rowWUpdateCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	int c = rowCount(parent),
		ret = 0;
	for(int i = 0; i < c; ++i)
	{
        if(parent.child(i, 0).flags() & Qt::ItemFlag(NvAbstractListItem::hasUpdate))
			++ret;
	}
	return ret;
}
