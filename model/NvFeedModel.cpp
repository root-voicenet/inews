#include "NvFeedModel.h"
#include "NvFeedCategory.h"
#include "../dbmanager.h"
#include "NvFeedItem.h"
#include <QModelIndex>
#include <QInputDialog>
#include <QDebug>
#include <QMimeData>

NvFeedModel::NvFeedModel(QObject *parent) :
    QAbstractItemModel(parent), m_categoryIcon(":/images/baloon.png"), m_pulledFeedName("")
{
    rootItem = new NvFeedCategory(0, "Root");
    addCategory(new NvFeedCategory(CATEGORY_ALL, "All"), rootItem);
}

NvFeedModel::~NvFeedModel()
{
    delete rootItem;
}

NvFeedCategory *NvFeedModel::rootCategory()
{
    return qobject_cast<NvFeedCategory*>(rootItem->child(0));
}

void NvFeedModel::setPulledFeed(const QString &name)
{
    m_pulledFeedName = name;
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
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    NvAbstractTreeItem *m = item(index);

    if(qobject_cast<NvFeedCategory*>(m)) {
        flags |= Qt::ItemIsDropEnabled;
    }else if(qobject_cast<NvFeedItem*>(m)) {
        flags |= Qt::ItemIsDragEnabled;
    }

    return flags;
}

QMimeData *NvFeedModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    foreach (QModelIndex index, indexes) {
       if (index.isValid()) {
           NvAbstractTreeItem *m = item(index);
           if(qobject_cast<NvFeedItem*>(m)) {
               stream << m->id();
           }
       }
    }
    mimeData->setData("inews/feeds", encodedData);
    return mimeData;
}

QStringList NvFeedModel::mimeTypes() const
{
    QStringList types;
    types << "inews/feeds";
    return types;
}

Qt::DropActions NvFeedModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

QVariant NvFeedModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
             return QVariant();

    if (role != Qt::DisplayRole)
             return QVariant();

    NvAbstractTreeItem *m = item(index);
    if(qobject_cast<NvFeedItem*>(m)) {
        return qobject_cast<NvFeedItem*>(m)->data( role );
    }

    if(qobject_cast<NvFeedCategory*>(m)) {
        return qobject_cast<NvFeedCategory*>(m)->data( role );
    }

    return QVariant();
}

bool NvFeedModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("inews/feeds"))
        return false;

    if(!parent.isValid())
        return false;

    NvFeedCategory *c = qobject_cast<NvFeedCategory*>(item(parent));

    if(!c)
        return false;

    QByteArray encodedData = data->data("inews/feeds");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    while (!stream.atEnd()) {
        int fid;
        stream >> fid;
        NvFeedItem *f = feed(fid);
        if(f) {
           addFeed(f, c);

           DBManager* db = DBManager::instance();
           NvFeedCategory* parentCat = qobject_cast<NvFeedCategory*>(c->parent());

           if(!db->storeFeedCategory(c->name(), parentCat->id(), c->feeds(), c->id())) {
               qDebug() << "Could not store feed";
               return false;
           }
        }
    }

    return true;
}

NvFeedCategory* NvFeedModel::category(int id)
{
    if(id == CATEGORY_ALL) {
        if(m_categories.contains(id))
            return m_categories[id];
    }

    return NULL;
}

NvAbstractTreeItem* NvFeedModel::item(const QModelIndex &index) const
{
    if (index.isValid()) {
        if(index.internalId() < (1 << magickNum())) {
            return qobject_cast<NvFeedCategory*>(m_categories[index.internalId()]);
        }else{
            int catID = index.internalId() & mask();
            NvFeedCategory *category = m_categories[catID];
            if(m_feeds.contains(catID)) {
                return m_feeds[catID].at(index.row() - category->childCount());
            }
        }
    }

    return NULL;
}

bool NvFeedModel::categoryIsValid(NvFeedCategory *item) const
{
    // TODO: return false if selected category "all"
    return true;
}

NvFeedCategory *NvFeedModel::addCategory(const QString &title, NvFeedCategory *parent)
{
    DBManager* db = DBManager::instance();
    int id = db->storeFeedCategory(title, parent->id());
    if(id) {
        NvFeedCategory *item = new NvFeedCategory(id, title);
        addCategory(item, parent);
        return item;
    }

    return NULL;
}

bool NvFeedModel::saveCategory(NvFeedCategory *item)
{
    NvFeedCategory* parent = qobject_cast<NvFeedCategory*>(item->parent());
    DBManager* db = DBManager::instance();
    return db->storeFeedCategory( item->name(), parent->id(), parent->feeds(), item->id() ) != 0;
}

bool NvFeedModel::init()
{
    // load stored categories
    DBManager* db = DBManager::instance();
    return db->loadFeedTree(this, rootItem);
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

        QMapIterator<int, NvFeedCategory*> ci(m_categories);
        NvFeedCategory *parent = NULL;
        while(ci.hasNext()) {
            NvFeedCategory* c = ci.next().value();
            if(c->feeds().indexOf(feed->id()) != -1) {
                parent = c;
            }
        }

        addFeed(feed, parent);
    }

    return false;
}

bool NvFeedModel::importFeed(QVariant *resp)
{
    QMap<QString, QVariant> elements(resp->toMap());
    NvFeedItem *newFeed = feed(NvFeedItem::NEW_FEED_ID);
    if(elements.contains("fid") && newFeed) {
        int fid = elements.value("fid").toInt();
        newFeed->setId(fid);
        return true;
    }

    return false;
}

void NvFeedModel::addFeed(NvFeedItem *item, NvFeedCategory *parent)
{
    if(parent) {
        parent->addFeed(item->id());
        if(m_feeds[parent->id()].indexOf(item) == -1)
            m_feeds[parent->id()].append(item);
    }

    if(m_feeds[CATEGORY_ALL].indexOf(item) == -1)
        m_feeds[CATEGORY_ALL].append(item);
}

NvFeedItem *NvFeedModel::feed(int id)
{
    ItemsList s = m_feeds[CATEGORY_ALL];

    foreach(NvAbstractTreeItem *feed, s) {
        NvFeedItem* c = qobject_cast<NvFeedItem*>(feed);

        if(c->id() == id)
            return c;
    }

    return NULL;
}

void NvFeedModel::deleteFeed(int id)
{
    /*
    foreach(ItemsList s, m_feeds) {
        for(int i = 0; i < s.size(); ++i) {
            NvAbstractTreeItem *item = s[i];
            if(item->id() == id ) {
                s.remove(i);
                delete item;
            }
        }
    }

    QMapIterator<int, NvFeedCategory*> i(m_categories);
    while(i.hasNext()) {
        NvFeedCategory *c = i.next().value();
        c->removeFid(id);
    }
    */
}
