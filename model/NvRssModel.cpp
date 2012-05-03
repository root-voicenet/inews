#include "NvRssModel.h"
#include "dbmanager.h"

NvRssModel::NvRssModel(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable(DBManager::RSS_TABLE);
    setRelation(DBManager::RSS_FIELD_FEED_ID, QSqlRelation(DBManager::FEEDS_TABLE, "id", "name"));

}


QVariant NvRssModel::data(const QModelIndex &index, int role) const
{
    if(index.column() == DBManager::RSS_FIELD_TITLE) {

    }else{
        return QSqlQueryModel::data(index, role);
    }

    return QVariant();

}
