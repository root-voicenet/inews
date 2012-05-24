#include "NvRssCachedModel.h"
#include "nvrssitem.h"
#include "NvFeedItem.h"
#include "../dbmanager.h"
#include "../resourcemanager.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>


NvRssCachedModel::NvRssCachedModel(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable(DBManager::RSS_TABLE);
    setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    setRelation(DBManager::RSS_FIELD_FEED_ID, QSqlRelation(DBManager::FEEDS_TABLE, "id", "title"));

}


QVariant NvRssCachedModel::data(const QModelIndex &index, int role) const
{
    QStringList tagsNames;
    QList<Tag> tags;

    if (!index.isValid()) {
            return QVariant();
    }

    if (index.column() == 0) {
        switch(role) {
            case Qt::DisplayRole:
                return record(index.row()).value(DBManager::RSS_FIELD_TITLE);
            break;
            case DescriptionRole:
                return record(index.row()).value(DBManager::RSS_FIELD_DESCRIPTION);
            break;
            case DateRole:
                return QDateTime::fromTime_t(record(index.row()).value(DBManager::RSS_FIELD_DATE).toUInt());
            break;
            case TagRole:
                tags = TagsManager::getRssTags( record( index.row() ).value(DBManager::RSS_FIELD_REMOTE_ID).toUInt() );
                foreach(Tag tag, tags)
                    tagsNames << tag.name();

                return tagsNames;
            break;
            case RssIdRole:
                return QSqlRelationalTableModel::data ( QSqlRelationalTableModel::index(index.row(),
                                                                                    DBManager::RSS_FIELD_REMOTE_ID), Qt::DisplayRole );
            break;
            case ReadedRole:
                return QSqlRelationalTableModel::data ( QSqlRelationalTableModel::index(index.row(),
                                                                                    DBManager::RSS_FIELD_READED), Qt::DisplayRole );
            break;

            case FeedRole:
                return QSqlRelationalTableModel::data ( QSqlRelationalTableModel::index(index.row(),
                                                                                        DBManager::RSS_FIELD_FEED_ID), Qt::DisplayRole );
                break;

            default:
                return QSqlRelationalTableModel::data ( index, role );
            break;
        }
    }

    return QSqlRelationalTableModel::data ( index, role );
}


NvRssItem NvRssCachedModel::item(const QModelIndex &index) const
{
    if(index.isValid()) {
        QSqlRecord r = record(index.row());

        NvRssItem rss(r.value(DBManager::RSS_FIELD_REMOTE_ID).toInt(),
                      r.value(DBManager::RSS_FIELD_TITLE).toString(),
                      r.value(DBManager::RSS_FIELD_DESCRIPTION).toString());

        rss.setDate(QDateTime::fromTime_t(r.value(DBManager::RSS_FIELD_DATE).toUInt()));
        rss.setReaded(r.value(DBManager::RSS_FIELD_READED).toBool());
        rss.setLink(r.value(DBManager::RSS_FIELD_LINK).toString());
        rss.setTags( TagsManager::getRssTags( r.value(DBManager::RSS_FIELD_REMOTE_ID).toUInt() ) );

        return rss;
    }

    return NvRssItem(0, "", "");
}

QList<Tag> NvRssCachedModel::parseTags(const QVariant *input)
{
    QList<QVariant> tids =  input->toList();
    QList<Tag> res;
    foreach(const QVariant& i, tids) {
        int tagId = qvariant_cast<int>(i);
        Tag tag = TagsManager::getTag( tagId );
        if(tag.isValid()) {
            res << tag;
        }
    }
    return res;
}

int NvRssCachedModel::parseOperation(const QString &opstring) const
{
    if(opstring == "insert")
        return OP_INSERT;
    if(opstring == "update")
        return OP_UPDATE;
    if(opstring == "delete")
        return OP_DELETE;

    return OP_INSERT;
}

bool NvRssCachedModel::import(const QVariant *in)
{
    ResourceManager *rm = ResourceManager::instance();

    NvFeedModel *feeds = rm->feedModel();

    QList<QVariant> elements(in->toList());

        for (int i = 0; i < elements.size(); ++i) {
            // parse element
            QMap<QString, QVariant> tags = elements[i].toMap();
            QString rssTitle, rssDescription, rssLink;
            QUrl imageUrl;

            // create view
            int rssId;
            uint cdate;
            QList<Tag> rssTags;
            quint32 fid;
            int op;
            int row;


            op = parseOperation(tags.value("op").toString());
            if(op == OP_INSERT || op == OP_UPDATE) {

                rssTitle = tags.value("title").toString();
                rssId = tags.value("iid").toInt();
                cdate = tags.value("date").toUInt();

                QList<QVariant> images = tags.value("image").toList();
                if(!images.isEmpty()) {
                    imageUrl = images.first().toString();
                }

                if(!tags.value("fid").isNull()) {
                    fid = tags.value("fid").toUInt();
                }

                if(!tags.value("description").isNull()) {
                    rssDescription = tags.value("description").toString();
                }

                if(!tags.value("tids").isNull()) {
                    rssTags = parseTags(&tags.value("tids"));
                }

                if(!tags.value("link").isNull()) {
                    rssLink = tags.value("link").toString();
                }

                if(rssTitle.isEmpty()) {
                    qDebug() << "Title is empty";
                    return false;
                }

                if(op == OP_INSERT) {
                    row = rowCount();
                    insertRow( row );
                }else if(op == OP_UPDATE) {
                    QModelIndexList matches = match(index(0, DBManager::RSS_FIELD_REMOTE_ID), Qt::DisplayRole, rssId);
                    if(matches.isEmpty())
                        continue;
                    row = matches.at(0).row();
                }

                setData(index(row, DBManager::RSS_FIELD_REMOTE_ID), rssId);
                setData(index(row, DBManager::RSS_FIELD_TITLE), rssTitle);
                setData(index(row, DBManager::RSS_FIELD_DESCRIPTION), rssDescription);
                setData(index(row, DBManager::RSS_FIELD_FEED_ID), fid);
                setData(index(row, DBManager::RSS_FIELD_DATE), cdate);
                setData(index(row, DBManager::RSS_FIELD_LINK), rssLink);

                TagsManager::setRssTags(rssId, rssTags);
            }else if(op == OP_DELETE) {
                QModelIndexList matches = match(index(0, DBManager::RSS_FIELD_REMOTE_ID), Qt::DisplayRole, rssId);
                if(matches.isEmpty())
                    continue;

                row = matches.at(0).row();

                removeRows(row, 1);
                TagsManager::setRssTags(rssId, rssTags);
            }else{
                qDebug() << "Unknown op when importion rss";
            }


        }

        database().transaction();
        if(submitAll()) {
            database().commit();
            DBManager::instance()->setUpdateTime(DBManager::TYPE_RSS, QDateTime::currentDateTime());
            return true;
        }else{
            database().rollback();
            qDebug() << "Can save new items";
        }

    return false;
}

bool NvRssCachedModel::save(const NvRssItem &item)
{

    NvRssCachedModel model;
    model.setFilter( QString("remote_id = %1").arg(item.id()) );
    model.select();
    if(model.rowCount() == 1) {
        model.setData(index(0, DBManager::RSS_FIELD_READED), item.readed());
        TagsManager::setRssTags( item.id(), item.tags() );
        model.submitAll();

        _updated << item;

        return true;
    }

   return false;
}


QList<NvRssItem> NvRssCachedModel::updatedItems() const
{
   return _updated;
}

void NvRssCachedModel::clearUpdated()
{
    _updated.clear();
}

NvRssItem NvRssCachedModel::search(quint32 id) const
{
    QModelIndexList matches = match(index(0, DBManager::NODE_FIELD_ID), RssIdRole, id);
    if(matches.isEmpty())
        return NvRssItem();

    return item(index(matches[0].row(), 0));
}

/*
void NvRssCachedModel::fetchMore(const QModelIndex & /* index )
{
    int remainder = localRssCount() - m_localCount;
    int itemsToFetch = qMin(m_buffersize, remainder);

    QList<NvRssItem*> l_items = m_storage->listRss(m_localCount, itemsToFetch);

    beginInsertRows(QModelIndex(), m_localCount, m_localCount + l_items.size() - 1);
    for(int j = 0; j < l_items.size(); ++j) {
        NvRssItem *item = l_items[j];

        item->setParent(this);
        connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(itemDeleted(QObject *)));

        items << item;

        bool ret = connect(item, SIGNAL(needUpdate()), this, SLOT(itemNeedUpdate()));
        Q_ASSERT(ret);
    }
    m_localCount += l_items.size();
    endInsertRows();

}
*/

/*
QList<NvRssItem*> NvRssCachedModel::updatedRss() const
{
    QListIterator< NvAbstractListItemPtr > i(items);
    QList<NvRssItem*> items;
    while(i.hasNext()) {
        NvRssItem *item = dynamic_cast<NvRssItem*>(i.next().data());
        if(item->updated())
            items << item;
    }
    return items;
}
*/

