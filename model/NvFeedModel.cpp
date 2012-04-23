#include "NvFeedModel.h"
#include "NvFeedCategory.h"
#include "../dbmanager.h"
#include "NvFeedItem.h"
#include <QModelIndex>
#include <QInputDialog>
#include <QDebug>

NvFeedModel::NvFeedModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootItem = new NvFeedCategory(0, "Root");
    addCategory(new NvFeedCategory(DEFAULT_CATEGORY_ID, "All", rootItem));
    NvFeedCategory* is = new NvFeedCategory(3, "Category 2");
    addCategory(is);
    addCategory(new NvFeedCategory(444, "Sub Hello"), is);
    addCategory(new NvFeedCategory(2, "Category 1", rootItem));

    ItemsList list;
    NvFeedItem *feed1 = new NvFeedItem(222, "Hello 1");
    list << feed1;
    list << new NvFeedItem(333, "Hello 2");

    m_feeds.insert(2, list);
    list.clear();
    list << feed1;
    m_feeds.insert(444, list);
    feed1->setTitle("viper");
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
        parentItem = reinterpret_cast<NvFeedCategory*>(m_categories[parent.internalId()]);

    if(row < parentItem->childCount()) {
        NvFeedCategory *childItem = qobject_cast<NvFeedCategory*>(parentItem->child(row));
        return createIndex(row, column, childItem->id());
    }else{
        int itemIdx = row - parentItem->childCount();
        int id = (itemIdx + 1) << magickNum();
        id = id | parentItem->id();

        if(m_feeds.contains(parentItem->id()) && itemIdx < m_feeds[parentItem->id()].count()) {
           return createIndex(row, column, id);
        }

    }
    return QModelIndex();
}

QModelIndex NvFeedModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if(index.internalId() >= (1 << magickNum()) ) {
        NvFeedCategory *parentItem = qobject_cast<NvFeedCategory*>(m_categories[index.internalId() & mask()]);
        if (parentItem == rootItem)
            return QModelIndex();
        return createIndex(parentItem->row(), 0, parentItem->id());
    }

    NvAbstractTreeItem *childItem = static_cast<NvAbstractTreeItem*>(m_categories[index.internalId()]);
    if(childItem) {
        NvFeedCategory *parentItem = qobject_cast<NvFeedCategory*>(childItem->parent());

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem->id());
    }

    return QModelIndex();
}

int NvFeedModel::rowCount(const QModelIndex &parent) const
{
    NvAbstractTreeItem *parentItem;

    if (parent.column() > 0)
            return 0;

    if (!parent.isValid()) {
        parentItem = rootItem;
    }else{
        if(parent.internalId() >= (1 << magickNum())) {
            return 0;
        }

        parentItem = reinterpret_cast<NvAbstractTreeItem*>(m_categories[parent.internalId()]);
    }
    if(qobject_cast<NvFeedCategory*>(parentItem)) {
        NvFeedCategory *category = qobject_cast<NvFeedCategory*>(parentItem);
        int count = category->childCount();
        if(m_feeds.contains( category->id() ))
            count += m_feeds[ category->id() ].size();
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

    if(index.internalId() >= (1 << magickNum())) {
        int catID = index.internalId() & mask();
        if(m_feeds.contains(catID)) {
            return m_feeds[catID].at(index.row())->data(role);
        }
        return QVariant();
    }

    NvFeedCategory *item = static_cast<NvFeedCategory*>(m_categories[index.internalId()]);

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
    NvFeedCategory* parent = qobject_cast<NvFeedCategory*>(item->parent());
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

void NvFeedModel::categoryDeleted(QObject *item)
{
    NvFeedCategory * category = qobject_cast<NvFeedCategory*>(item);
    if(m_categories.contains(category->id())) {
        m_categories.remove(category->id());
    }
}

void NvFeedModel::addCategory(NvFeedCategory *item, NvFeedCategory *parent)
{
    if(!parent)
        parent = rootItem;
    parent->appendChild(item);
    bool ret = connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(categoryDeleted(QObject*)));
    Q_ASSERT(ret);

    m_categories.insert(item->id(), item);
}

void NvFeedModel::clearFeeds()
{
    QMapIterator<int, ItemsList> i(m_feeds);
    while(i.hasNext()) {
        qDeleteAll(i.next().value());
    }
    m_feeds.clear();
}

bool NvFeedModel::importFeeds(QVariant *resp)
{
    QList<QVariant> elements(resp->toList());
    clearFeeds();

    for (int i = 0; i < elements.size(); ++i) {
        // parse element
        QMap<QString, QVariant> tags = elements[i].toMap();
        QString feedTitle;
        int fid = 0;

        fid = tags.value("fid").toInt();
        if(!fid) {
            qDebug() << "Feed Id is empty";
            clearFeeds();
            return false;
        }

        feedTitle = tags.value("title").toString();
        if(feedTitle.isEmpty()) {
            qDebug() << "Feed Title is empty";
            clearFeeds();
            return false;
        }

        NvFeedItem *feed = new NvFeedItem(fid, feedTitle);
        addFeed(feed);
    }

    return false;
}

void NvFeedModel::addFeed(NvFeedItem *item)
{
    m_feeds[DEFAULT_CATEGORY_ID].append(item);
}
