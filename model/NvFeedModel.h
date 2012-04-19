#ifndef NVFEEDMODEL_H
#define NVFEEDMODEL_H

#include <QAbstractItemModel>

QT_FORWARD_DECLARE_CLASS(NvFeedCategory)
QT_FORWARD_DECLARE_CLASS(NvFeedItem)

class NvAbstractTreeItem;
class NvFeedCategory;
class NvFeedItem;
class NvFeedModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit NvFeedModel(QObject *parent = 0);
    ~NvFeedModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    NvAbstractTreeItem *item(const QModelIndex &index);


public:
    bool init();
    bool categoryIsValid(NvFeedCategory *item) const;
    bool saveCategory(NvFeedCategory* item);
private:
    NvFeedCategory *rootItem;
    QMap< int, QList<NvFeedItem*> > m_feeds;
signals:

public slots:

};

#endif // NVFEEDMODEL_H
