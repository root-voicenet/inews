#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "model/NvRssItem.h"
#include <QSqlDatabase>

class DBManager
{
public:
    DBManager();
    ~DBManager();

    static DBManager* instance();
    bool init(const QString& databasePath = "");

    QList<NvRssItem*> listRss();

    // manage functions

    // return local rss id if sucess, 0 if false
    int addRss( const NvRssItem& rss );
    void clearRss();
    bool editRss( const NvRssItem &newRss, int rssId );
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
