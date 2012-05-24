#include "NvNodeModel.h"
#include "NvMediaModel.h"
#include "../dbmanager.h"
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

NvNodeModel::NvNodeModel(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable(DBManager::NODES_TABLE);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setSortMode(SORT_DATE_DSC);
    select();

}


QVariant NvNodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
            return QVariant();
    }

    if (index.column() == 0) {
        switch(role) {
            case Qt::DisplayRole:
                return record(index.row()).value(DBManager::NODE_FIELD_TITLE);
            break;
            case NodeIDRole:
                 return record(index.row()).value(DBManager::NODE_FIELD_ID);
            break;
             default:
                return QSqlTableModel::data ( index, role );
            break;
        }
    }

    return QSqlTableModel::data ( index, role );
}


Node NvNodeModel::item(const QModelIndex &index) const
{
    if(index.isValid()) {
        QSqlRecord r = record(index.row());

        Node node(r.value(DBManager::NODE_FIELD_ID).toUInt(),
                      r.value(DBManager::NODE_FIELD_TITLE).toString(),
                      r.value(DBManager::NODE_FIELD_DESCRIPTION).toString());

        node.setDate(QDateTime::fromTime_t(r.value(DBManager::NODE_FIELD_DATE).toUInt()));
        node.setStatus(r.value(DBManager::NODE_FIELD_STATUS).toInt());
        node.setTags( TagsManager::getNodeTags( r.value(DBManager::NODE_FIELD_ID).toUInt() ) );
        node.setAttached( MediaManager::getNodeMedia( r.value(DBManager::NODE_FIELD_ID).toUInt() ));

        return node;
    }

    return Node(0, "");
}

QList<Tag> NvNodeModel::parseTags(const QVariant *input)
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

int NvNodeModel::parseOperation(const QString &opstring) const
{
    if(opstring == "insert")
        return OP_INSERT;
    if(opstring == "update")
        return OP_UPDATE;
    if(opstring == "delete")
        return OP_DELETE;

    return OP_INSERT;
}

bool NvNodeModel::import(const QVariant *in)
{
    QList<QVariant> elements(in->toList());

     for (int i = 0; i < elements.size(); ++i) {
            // parse element
            QMap<QString, QVariant> tags = elements[i].toMap();
            QString title;

            // create view
            quint32 node_id;
            uint cdate;
            QList<Tag> nodeTags;
            bool promoted = false;
            int op;
            int row;

            op = parseOperation(tags.value("op").toString());
            if(op == OP_INSERT || op == OP_UPDATE) {

                title = tags.value("title").toString();
                node_id = tags.value("nid").toUInt();
                cdate = tags.value("date").toUInt();

                qDebug() << "tvoe: " << tags.value("tvoe").toString();
                qDebug() << "moe: " << tags.value("moe").toString();

                if(!tags.value("promoted").isNull()) {
                    promoted = tags.value("promoted").toUInt();
                }


                if(!tags.value("tids").isNull()) {
                    nodeTags = parseTags(&tags.value("tids"));
                }

                if(title.isEmpty()) {
                    qDebug() << "Title is empty";
                    return false;
                }

                Node node = search(node_id);
                if(node.id()) {
                    op = OP_UPDATE;
                }


                if(op == OP_INSERT) {

                    row = rowCount();
                    insertRow( row );

                    setData(index(row, DBManager::NODE_FIELD_ID), node_id);
                    setData(index(row, DBManager::NODE_FIELD_TITLE), title);
                    setData(index(row, DBManager::NODE_FIELD_PROMOTED), promoted);
                    setData(index(row, DBManager::NODE_FIELD_DATE), cdate);
                    setData(index(row, DBManager::NODE_FIELD_STATUS), Node::STATUS_OK );

                }else if(op == OP_UPDATE) {
                        node.setTitle(title);
                        node.setPromoted(promoted);
                        node.setStatus( Node::STATUS_OK );
                        save(node);
                }

                TagsManager::setNodeTags(node_id, nodeTags);

            }else if(op == OP_DELETE) {
                QModelIndexList matches = match(index(0, DBManager::NODE_FIELD_ID), NodeIDRole, node_id);
                if(matches.isEmpty())
                    continue;

                row = matches.at(0).row();

                removeRows(row, 1);
                TagsManager::setNodeTags(node_id, nodeTags);
            }else{
                qDebug() << "Unknown op when importion nodes";
            }


        }

        database().transaction();
        if(submitAll()) {
            database().commit();
            select();
            DBManager::instance()->setUpdateTime(DBManager::TYPE_NODE, QDateTime::currentDateTime());
            return true;
        }else{
            database().rollback();
            qDebug() << "Can save new node";
        }

    return false;
}

Node NvNodeModel::search(quint32 node_id) const
{
    NvNodeModel model;
    model.setFilter(QString("id = %1").arg(node_id));
    model.select();
    if(model.rowCount() == 1) {
       return item(index(0, 0));
    }

    return Node(0, "");
}

bool NvNodeModel::save(const Node &item)
{
   QModelIndexList matches = match(index(0, DBManager::NODE_FIELD_ID), NodeIDRole, item.id());

   if(!matches.isEmpty()) {
       int row = matches.at(0).row();

       setData(index(row, DBManager::NODE_FIELD_ID), item.id());
       setData(index(row, DBManager::NODE_FIELD_TITLE), item.title());
       setData(index(row, DBManager::NODE_FIELD_PROMOTED), item.promoted());
       setData(index(row, DBManager::NODE_FIELD_DATE), item.date().toTime_t());
       setData(index(row, DBManager::NODE_FIELD_DESCRIPTION), item.getBody());
       setData(index(row, DBManager::NODE_FIELD_STATUS), item.status());

       TagsManager::setNodeTags( item.id(), item.tags() );
       MediaManager::setNodeMedia( item.id(), item.attachedMedia() );
       setNodeRss( item.id(), item.attachedRss());

       _updated << item;

       return true;
   }

   return false;
}

bool NvNodeModel::add(const Node &node)
{
   quint32 node_id = DBManager::instance()->maxNodeId() + 1;

    int row = rowCount();
    if( insertRow( row )) {
        setData(index(row, DBManager::NODE_FIELD_ID), node_id );
        setData(index(row, DBManager::NODE_FIELD_TITLE), node.title());
        setData(index(row, DBManager::NODE_FIELD_PROMOTED), node.promoted());
        setData(index(row, DBManager::NODE_FIELD_DATE), node.date().toTime_t());
        setData(index(row, DBManager::NODE_FIELD_DATE), node.getBody());

        TagsManager::setNodeTags( node_id, node.tags() );
        MediaManager::setNodeMedia( node.id(), node.attachedMedia() );

         _updated << node;

        return true;
    }

    return false;
}

QList<Node> NvNodeModel::composedItems() const
{
    QList<Node> res;
    NvNodeModel model;
    model.setFilter("status is null or status = 0");
    model.select();
    if(model.rowCount() > 0) {
        for(int i = 0; i < model.rowCount(); ++i) {
            res << model.item(model.index(i, 0));
        }
    }

   return res;
}

void NvNodeModel::clearUpdated()
{
    _updated.clear();
}


void NvNodeModel::setSortMode(int sortMode)
{
    if(sortMode == SORT_DATE_ASC || sortMode == SORT_DATE_DSC) {
        setSort(DBManager::NODE_FIELD_DATE, sortMode == SORT_DATE_ASC ? Qt::AscendingOrder : Qt::DescendingOrder);
    }else if(sortMode == SORT_TITLE_ASC || sortMode == SORT_TITLE_DSC) {
        setSort(DBManager::NODE_FIELD_TITLE, sortMode == SORT_TITLE_ASC ? Qt::AscendingOrder : Qt::DescendingOrder);
    }
}


void NvNodeModel::setNodeRss(quint32 node_id, QList<quint32> items)
{
    QSqlQuery query(DBManager::instance()->connection());
    query.exec(QString("delete from " + DBManager::NODES_RSS_TABLE + " where node_id = %1").arg(node_id));

    for(int i = 0; i < items.size(); ++i) {
        query.prepare("insert into " + DBManager::NODES_RSS_TABLE + " (rss_id, node_id) VALUES(?, ?)");
        query.addBindValue( items[i] );
        query.addBindValue( node_id );

        if(!query.exec()) {
           qDebug() << "Error executing query: " << query.lastError().driverText();
           break;
        }
    }
}

QList<quint32> NvNodeModel::getNodeRss(quint32 node_id)
{
    QList<quint32> res;
    QSqlQuery query(DBManager::instance()->connection());

    if(!query.exec(QString("SELECT rss_id FROM " + DBManager::NODES_FILES_TABLE + " WHERE node_id = %1").arg(node_id))) {
        qDebug() << "Error executing query: " << query.lastError().driverText();
        return res;
    }
    while (query.next()){
        res << query.value(0).toUInt();
    }

    return res;
}
