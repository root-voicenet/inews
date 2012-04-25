#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "model/nvrssitem.h"
#include <QSqlDatabase>

class NvFeedModel;
class NvFeedCategory;
class DBManager
{
public:
    DBManager();
    ~DBManager();

    static DBManager* instance();
    bool init(const QString& databasePath = "");

    QList<NvLocalRssItem*> listRss(int start, int count);
    bool loadFeedTree( NvFeedModel *model, NvFeedCategory *root );
    int storeFeedCategory( const QString &title, int parent_id = false, QList<int> fids = QList<int>(), int id = 0);

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
    bool storeCategoryFids(int id, QList<int> fids);
    static DBManager* m_instance;
    QString m_lastError;
    QString m_dbPath;

    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
