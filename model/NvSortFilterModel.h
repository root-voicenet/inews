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
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
public slots:
    void setFeedId(int id);

private:
    int m_fid;
};
#endif // NVSORTFILTERMODEL_H
