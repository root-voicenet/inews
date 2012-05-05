#ifndef NVFEEDMODEL_H
#define NVFEEDMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QIcon>
#include "NvFeedCategory.h"

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

    enum {
        CATEGORY_ALL = 8000
    };

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                          int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions() const;
    QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    NvAbstractTreeItem *item(const QModelIndex &index) const;

public:
    bool init();
    void clearFeeds();
    bool categoryIsValid(NvFeedCategory *item) const;
    bool saveCategory(NvFeedCategory* item);
    void addCategory(NvFeedCategory *item, NvFeedCategory *parent = 0);
    bool importFeeds(QVariant *resp);
    bool importFeed(QVariant *resp);
    void setPulledFeed(const QString& name);
    void deleteFeed(int id);
    NvFeedCategory *rootCategory();


    NvFeedItem* feed( int id );
    NvFeedCategory *category(int id);
    NvFeedCategory *addCategory(const QString& title, NvFeedCategory *parent);
    void addFeed(NvFeedItem *item, NvFeedCategory *parent = NULL);
private:   

    typedef QVector<NvAbstractTreeItemPtr> ItemsList;

    NvFeedCategory *rootItem;
    QMap<int, ItemsList> m_feeds;
    QMap<int, NvFeedCategory*> m_categories;
    QIcon m_categoryIcon;
    QString m_pulledFeedName;


    inline int magickNum() const
    {
       return 16;
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
signals:

public slots:
    void categoryDeleted(QObject *item);
};

#endif // NVFEEDMODEL_H
