#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "model/nvrssitem.h"
#include <QSqlDatabase>

class NvFeedModel;
class NvFeedCategory;
class DBManager
{
public:
    static const QString RSS_TABLE;
    static const QString RSS_TAGS_TABLE;
    static const QString FEEDS_TABLE;
    static const QString TAGS_TABLE;
    static const QString TIME_TABLE;
    static const QString NODES_TABLE;
    static const QString NODES_TAGS_TABLE;
    static const QString NODES_RSS_TABLE;
    static const QString NODES_FILES_TABLE;
    static const QString FILES_TABLE;

    static QDateTime cached_updates[10];

    enum RssFields {
        RSS_FIELD_ID = 0,
        RSS_FIELD_REMOTE_ID,
        RSS_FIELD_FEED_ID,
        RSS_FIELD_TITLE,
        RSS_FIELD_DESCRIPTION,
        RSS_FIELD_DATE,
        RSS_FIELD_LINK,
        RSS_FIELD_READED
    };

    enum NodeFields {
        NODE_FIELD_ID = 0,
        NODE_FIELD_TITLE,
        NODE_FIELD_DESCRIPTION,
        NODE_FIELD_DATE,
        NODE_FIELD_PROMOTED,
        NODE_FIELD_STATUS
    };



    // for storing last updated
    enum StoreTime {
        TYPE_RSS = 1,
        TYPE_NODE
    };


    DBManager();
    ~DBManager();

    static DBManager* instance();
    bool init(const QString& databasePath = "");

    QList<NvRssItem*> listRss(int start, int count);

    bool loadFeedTree( NvFeedModel *model, NvFeedCategory *root );
    int storeFeedCategory( const QString &title, int parent_id = false, QList<int> fids = QList<int>(), int id = 0);
    void clearFeeds();
    bool addFeed(quint32 id, const QString& title);

    // manage functions

    // return local rss id if sucess, 0 if false
    bool storeRss( const NvRssItem *item );
    quint32 maxNodeId() const;


    int rssCount();
    void clearRss();
    bool editRss( const NvRssItem &newRss, int rssId );
    bool removeRss( int rssId );


    void setUpdateTime(DBManager::StoreTime type, QDateTime time);
    QDateTime getUpdateTime(DBManager::StoreTime type);
    QSqlDatabase connection();
private:
    bool connectDB();
    bool createDB();
    bool storeCategoryFids(int id, QList<int> fids);
    static DBManager* m_instance;
    QString m_lastError;
    QString m_dbPath;

    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
