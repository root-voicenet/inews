#include "NvFeedModel.h"
#include "NvFeedCategory.h"
#include "../dbmanager.h"
#include "NvFeedItem.h"
#include <QModelIndex>
#include <QInputDialog>

NvFeedModel::NvFeedModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootItem = new NvFeedCategory(0, "Root");
    rootItem->appendChild( new NvFeedCategory(1, "All", rootItem));

    QList<NvFeedItem*> items;
    items << new NvFeedItem(111, "Hello");
    m_feeds.insert(1, items);
}

NvFeedModel::~NvFeedModel()
{
    delete rootItem;
}

int NvFeedModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex NvFeedModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
            return QModelIndex();

    NvFeedCategory *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<NvFeedCategory*>(parent.internalPointer());

    NvFeedCategory *childItem = dynamic_cast<NvFeedCategory*>(parentItem->child(row));
    if (childItem)
        return createIndex(row, column, childItem);
    else{
        NvFeedItem *item = 0;
        if(m_feeds.contains(parentItem->id()) && row < m_feeds[ parentItem->id() ].count()) {
            item = m_feeds[ parentItem->id() ].at(row);
        }
        if(item)
            return createIndex(row, column, item);
    }
    return QModelIndex();
}

QModelIndex NvFeedModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    NvAbstractTreeItem *childItem = static_cast<NvAbstractTreeItem*>(index.internalPointer());
    if(childItem) {
        NvFeedCategory *parentItem = dynamic_cast<NvFeedCategory*>(childItem->parent());

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
    }

    return QModelIndex();
}

int NvFeedModel::rowCount(const QModelIndex &parent) const
{
    NvAbstractTreeItem *parentItem;
    if (parent.column() > 0)
            return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<NvAbstractTreeItem*>(parent.internalPointer());

    if(dynamic_cast<NvFeedCategory*>(parentItem)) {
        NvFeedCategory *category = dynamic_cast<NvFeedCategory*>(parentItem);
        int count = category->childCount();
       // if(m_feeds.contains( category->id() ))
       //     count += m_feeds[ category->id() ].size();
        return count;
    }

    return 0;
}

QVariant NvFeedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return rootItem->data(role);

    return QVariant();
}

Qt::ItemFlags NvFeedModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant NvFeedModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
             return QVariant();

    if (role != Qt::DisplayRole)
             return QVariant();

    NvFeedCategory *item = static_cast<NvFeedCategory*>(index.internalPointer());

    return item->data(role);
}

NvAbstractTreeItem* NvFeedModel::item(const QModelIndex &index)
{
    if (index.isValid()) {
        return static_cast<NvFeedCategory*>(index.internalPointer());
    }

    return NULL;
}

bool NvFeedModel::categoryIsValid(NvFeedCategory *item) const
{
    // TODO: return false if selected category "all"
    return true;
}

bool NvFeedModel::saveCategory(NvFeedCategory *item)
{
    NvFeedCategory* parent = dynamic_cast<NvFeedCategory*>(item->parent());
    if(!item->id()) {
        parent->appendChild(item);
    }

    DBManager* db = DBManager::instance();
    return db->storeFeedCategory( item, item->id() != 0);
}

bool NvFeedModel::init()
{
    // load stored categories
    DBManager* db = DBManager::instance();
    return db->loadFeedTree(rootItem);
}
