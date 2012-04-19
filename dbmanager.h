#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "model/nvrssitem.h"
#include <QSqlDatabase>

class NvFeedCategory;
class DBManager
{
public:
    DBManager();
    ~DBManager();

    static DBManager* instance();
    bool init(const QString& databasePath = "");

    QList<NvLocalRssItem*> listRss(int start, int count);
    bool loadFeedTree( NvFeedCategory *root );
    bool storeFeedCategory( NvFeedCategory* cat, bool update = false);

    // manage functions

    // return local rss id if sucess, 0 if false
    bool storeRss( const NvRemoteRssItem *item );
    int rssCount();
    void clearRss();
    bool editRss( const NvBaseItem &newRss, int rssId );
    bool removeRss( int rssId );

private:
    bool connectDB();
    bool createDB();
    static DBManager* m_instance;
    QString m_lastError;
    QString m_dbPath;

    QSqlDatabase m_db;
};

#endif // DBMANAGER_H