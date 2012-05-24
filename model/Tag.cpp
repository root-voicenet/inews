#include "Tag.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Tag TagsManager::_invalid(0, "");
TagsTreeModel TagsManager::knownTags;

class TagInternal {
public:
    quint32 id;
    quint32 parent;
    QString name;
    QStandardItem* treeItem;
};

bool Tag::operator==(const Tag &tag) const
{
    return id() == tag.id();
}

Qt::ItemFlags TagsTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QStandardItemModel::flags(index);

    if (!index.isValid())
        return flags;

    // is it category item?
    if(!index.parent().isValid()) {
        flags &= ~Qt::ItemIsSelectable;
        flags &= ~Qt::ItemIsEnabled;
    }

    return flags;
}


void TagsManager::init()
{
    cleanup();

    QList<TagInternal> items;
    QMap<quint32, TagInternal*> refs;

    QSqlQuery query(DBManager::instance()->connection());
    query.exec("select remote_id, tag, parent from tags");
    while (query.next()) {
        TagInternal ts;
        ts.id = query.value(0).toUInt();
        ts.name = query.value(1).toString();
        ts.parent = query.value(2).toUInt();
        ts.treeItem = new QStandardItem(ts.name);
        items << ts;
        refs[ts.id] = &items.last();
    }

    QStandardItem *root = knownTags.invisibleRootItem();
    for(int i = 0; i < items.size(); ++i) {
        QStandardItem *croot = root;
        if(items[i].parent && refs.contains(items[i].parent)) {
            croot = refs[ items[i].parent ]->treeItem;
        }

        croot->appendRow( items[i].treeItem );
    }
}

void TagsManager::cleanup()
{
    QStandardItem *root = knownTags.invisibleRootItem();
    root->removeRows(0, root->rowCount());
}

Tag TagsManager::getTag(const QString &tagString)
{
    QSqlQuery query(DBManager::instance()->connection());

    if(!query.exec(QString("select remote_id, tag, parent from tags where tag = '%1'").arg(tagString))) {
        qDebug() << query.lastError().databaseText() ;
    }
    if (!query.next()) return _invalid;
    return Tag(query.value(0).toUInt(), query.value(1).toString(), query.value(2).toUInt());
}

Tag TagsManager::getTag(quint32 id)
{
    QSqlQuery query(DBManager::instance()->connection());

    query.exec(QString("select remote_id, tag, parent from tags where remote_id = %1").arg(id));
    if (!query.next()) return _invalid;
    return Tag(query.value(0).toUInt(), query.value(1).toString(), query.value(2).toUInt());
}

Tag TagsManager::getOrCreateTag(const QString &tagString)
{
    Tag tag(getTag(tagString));
    if (tag.isValid()) return tag;

   QSqlQuery query(DBManager::instance()->connection());
    if (!query.exec(QString("insert into tags values('%1', 0)").arg(tagString))) {
        qCritical() << "Error executing query: " << query.lastError().databaseText();
        return _invalid;
    }
    if (!query.exec(QString("select remote_id, parent from tags where tag = '%1'").arg(tagString))) {
        qCritical() << "Error executing query: " << query.lastError().databaseText();
        return _invalid;
    }
    query.next();

    int id = query.value(0).toUInt();
    knownTags.invisibleRootItem()->appendRow(new QStandardItem(tagString));
    return Tag(id, tagString, query.value(1).toUInt());
}



bool TagsManager::isValidTag(const QString &string)
{
    if (string.isEmpty()) return false;

    //return (TextTools::isRomaji(string) || TextTools::isJapanese(string));

    // A tag is valid if it is made of letters, digits, or Japanese characters
    foreach(const QChar &c, string) {
        switch (c.category()) {
            case QChar::Punctuation_InitialQuote:
            case QChar::Punctuation_FinalQuote:
            case QChar::Separator_Space:
            case QChar::Separator_Line:
                return false;
            default:
                continue;
        }
    }
    return true;
}

bool TagsManager::import(QVariant *resp)
{
    QSqlQuery query(DBManager::instance()->connection());
    query.exec("delete from tags where 1");


    QList<QVariant> elements(resp->toList());

    for(int i = 0; i < elements.size(); ++i) {

        QMap<QString, QVariant> tags = elements[i].toMap();

        QString name = tags.value("name").toString();
        quint32 parent = 0;
        // build a parents
        QList<QVariant> parents = tags.value("parents").toList();
        if(parents.size() > 0)
            parent = parents.first().toUInt();

        quint32 remote_id = tags.value("tid").toUInt();

        query.prepare("insert into tags (remote_id, tag, parent) values (?, ?, ?)");
        query.addBindValue(remote_id);
        query.addBindValue(name);
        query.addBindValue(parent);

        if(!query.exec()) {
             qDebug() << "Error executing query: " << query.lastError().driverText();
             return false;
        }
    }

    init();
    return true;
}

QList<Tag> TagsManager::getRssTags(quint32 rss_id)
{
    QList<Tag> res;
    QSqlQuery query(DBManager::instance()->connection());

    query.exec(QString("SELECT t.remote_id, t.tag, t.parent FROM " + DBManager::RSS_TAGS_TABLE + " r INNER JOIN " + DBManager::TAGS_TABLE + " t ON(r.tid = t.remote_id) WHERE r.rss_id = %1").arg(rss_id));
    while (query.next()){
        res << Tag(query.value(0).toUInt(), query.value(1).toString(), query.value(2).toUInt());
    }

    return res;
}

void TagsManager::setRssTags(quint32 rss_id, QList<Tag> tags)
{
    QSqlQuery query(DBManager::instance()->connection());
    query.exec(QString("delete from " + DBManager::RSS_TAGS_TABLE + " where rss_id = %1").arg(rss_id));

    for(int i = 0; i < tags.size(); ++i) {
        query.prepare("insert into " + DBManager::RSS_TAGS_TABLE + " (tid, rss_id) VALUES(?, ?)");
        query.addBindValue( tags[i].id() );
        query.addBindValue( rss_id );

        if(!query.exec()) {
           qDebug() << "Error executing query: " << query.lastError().driverText();
           break;
        }
    }
}

QList<Tag> TagsManager::getNodeTags(quint32 node_id)
{
    QList<Tag> res;
    QSqlQuery query(DBManager::instance()->connection());

    query.exec(QString("SELECT t.remote_id, t.tag, t.parent FROM " + DBManager::NODES_TAGS_TABLE + " r INNER JOIN " + DBManager::TAGS_TABLE + " t ON(r.tid = t.remote_id) WHERE r.node_id = %1").arg(node_id));
    while (query.next()){
        res << Tag(query.value(0).toUInt(), query.value(1).toString(), query.value(2).toUInt());
    }

    return res;
}

void TagsManager::setNodeTags(quint32 node_id, QList<Tag> tags)
{
    QSqlQuery query(DBManager::instance()->connection());
    query.exec(QString("delete from " + DBManager::NODES_TAGS_TABLE + " where node_id = %1").arg(node_id));

    for(int i = 0; i < tags.size(); ++i) {
        query.prepare("insert into " + DBManager::NODES_TAGS_TABLE + " (tid, node_id) VALUES(?, ?)");
        query.addBindValue( tags[i].id() );
        query.addBindValue( node_id );

        if(!query.exec()) {
           qDebug() << "Error executing query: " << query.lastError().driverText();
           break;
        }
    }
}
