#ifndef NVSORTFILTERMODEL_H
#define NVSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class NvSortFilterModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
    NvSortFilterModel(QObject * parent);
    ~NvSortFilterModel(void);

	void setSourceModel ( QAbstractItemModel * sourceModel );

signals:
	void needUpdate(const QModelIndex & index);

public slots:
	void install(const QModelIndex & index );
	void remove(const QModelIndex & index );
	void updateItem(const QModelIndex & index );
	void enable(const QModelIndex & index );
	void disable(const QModelIndex & index );
	void detail( const QModelIndex & index );
	void cancel( const QModelIndex & index );

private slots:
	void needUpdate_src(const QModelIndex & index);

protected:
	bool lessThan ( const QModelIndex & left, const QModelIndex & right ) const;
};
#endif // NVSORTFILTERMODEL_H
