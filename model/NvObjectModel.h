#ifndef NVOBJECTMODEL_H
#define NVOBJECTMODEL_H

#include "NvAbstractListItem.h"
#include <QAbstractListModel>
#include <QUrl>
#include <QVector>
#include <QStringList>
#include <QImage>


class NvObjectModel : public QAbstractListModel
{
	Q_OBJECT

public:

	enum ObjectDataRole
	{
		DescriptionRole = Qt::UserRole + 1,
		DetailRole,
		VersionRole,
		InstalledRole,
		EnabledRole,
		BuildInRole,
		SourceRole,
		IconRole,
		DateRole,
		GuidRole,
		ProgressRole,
		NeedUpdateRole
	};

    NvObjectModel(QObject *parent);
    ~NvObjectModel();

    int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    NvAbstractListItem *item ( const QModelIndex & index ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );


    bool addItem( NvAbstractListItem * item);

    Qt::ItemFlags flags ( const QModelIndex & index ) const;


signals:
	void needUpdate(QModelIndex);

public slots:

private slots:
	void itemNeedUpdate();
	void itemDeleted(QObject * item);

protected:
    typedef QList< NvAbstractListItemPtr > ItemVector;
	
        ItemVector items;

        virtual NvAbstractListItem* getItem( const QModelIndex & index ) const;

	inline int magickNum() const
	{
		return items.count() % 2 + items.count() / 2;
    }

	inline int mask() const
	{
		int m = 1;
		int c = magickNum();
		for (int i = 1; i < c; ++i)
		{
			m = (m << 1) | 1;
		}
		return m;
    }

    QVariant itemData(int row, int role) const;
};

#endif // NVOBJECTMODEL_H
