#include "dbmanager.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QVariant>
#include "model/NvFeedCategory.h"
#include "model/NvFeedModel.h"

#include <sys/time.h>

#define DATABASE_DEFAULT_PATH "local.db"

const QString DBManager::RSS_TABLE = "rss_items";
const QString DBManager::RSS_TAGS_TABLE = "rss_tags";
const QString DBManager::FEEDS_TABLE = "feeds";
const QString DBManager::TAGS_TABLE = "tags";
const QString DBManager::TIME_TABLE = "updates";
const QString DBManager::NODES_TABLE = "nodes";
const QString DBManager::NODES_TAGS_TABLE = "nodes_tags";
const QString DBManager::NODES_RSS_TABLE = "nodes_rss";
const QString DBManager::NODES_FILES_TABLE = "nodes_files";
const QString DBManager::FILES_TABLE = "files";

QDateTime DBManager::cached_updates[10];

DBManager *DBManager::m_instance = NULL;

struct FeedCategoryInternal {
    QString title;
    int id;
    int parent;
    NvFeedCategory *item;
};

DBManager::DBManager()
{

}

DBManager::~DBManager()
{

}

DBManager *DBManager::instance()
{
    if ( !m_instance )
        m_instance = new DBManager;
    return m_instance;
}

bool DBManager::init(const QString &databasePath)
{
    QString path;

    if(databasePath.isEmpty())
        path = DATABASE_DEFAULT_PATH;
    else
        path = databasePath;

    m_dbPath = path;

    if ( !QFile::exists( path ) ) {
        if ( !createDB() ) {
            QMessageBox::critical(0, QApplication::tr( "Cannot create database" ),
                                        m_lastError );
            qDebug() << "Cannot create database, SQL error: " << m_lastError << endl;
            return false;
        }
    } else if ( !connectDB() ) {
        return false;
    }

    return true;
}

bool DBManager::createDB()
{
    bool ret = true;

    if ( !connectDB() )
       return false;

    QSqlQuery q;

    // Rss table
    if(!q.exec("CREATE TABLE " + RSS_TABLE + " (id INTEGER PRIMARY KEY, remote_id INTEGER, feed_id, title TEXT NOT NULL, description TEXT, created DATETIME NOT NULL, link TEXT, readed INTEGER)")) {
        ret = false;

        m_lastError = q.lastQuery();
    }

    // time table. stores last updates
    if(!q.exec("CREATE TABLE " + TIME_TABLE + " (id INTEGER PRIMARY KEY, type INTEGER NOT NULL, updated DATETIME NOT NULL)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // tags table
    if(!q.exec("CREATE TABLE " + TAGS_TABLE + " (id INTEGER PRIMARY KEY, remote_id INTEGER NOT NULL, tag VARCHAR(140) NOT NULL, parent INTEGER)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // rss tags table
    if(!q.exec("CREATE TABLE " + RSS_TAGS_TABLE + " (id INTEGER PRIMARY KEY, tid INTEGER NOT NULL, rss_id INTEGER NOT NULL)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // feeds table
    if(!q.exec("CREATE TABLE " + FEEDS_TABLE + " (id INTEGER NOT NULL, title TEXT NOT NULL)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // nodes table
    if(!q.exec("CREATE TABLE " + NODES_TABLE + " (id INTEGER NOT NULL, title TEXT NOT NULL, description TEXT, created DATETIME NOT NULL, promoted INTEGER NOT NULL, status INTEGER)")) {
        ret = false;

        m_lastError = q.lastQuery();
    }

    // nodes tags table
    if(!q.exec("CREATE TABLE " + NODES_TAGS_TABLE + " (id INTEGER PRIMARY KEY, tid INTEGER NOT NULL, node_id INTEGER NOT NULL)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // attached rss to nodes table
    if(!q.exec("CREATE TABLE " + NODES_RSS_TABLE + " (id INTEGER PRIMARY KEY, rss_id INTEGER NOT NULL, node_id INTEGER NOT NULL)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // attached files to nodes table
    if(!q.exec("CREATE TABLE " + NODES_FILES_TABLE + " (id INTEGER PRIMARY KEY, file_id INTEGER NOT NULL, node_id INTEGER NOT NULL, title TEXT, description TEXT)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    // files table
    if(!q.exec("CREATE TABLE " + FILES_TABLE + " (id INTEGER NOT NULL, name TEXT NOT NULL, url TEXT, localpath TEXT)")) {
        ret = false;
        m_lastError = q.lastQuery();
    }

    if(!q.exec("CREATE TABLE feed_category (id INTEGER PRIMARY KEY, title TEXT NOT NULL, parent INTEGER)")) {
        ret = false;

        m_lastError = q.lastQuery();
    }

    if(!q.exec("CREATE TABLE category_fids (id INTEGER PRIMARY KEY, category_id INTEGER NOT NULL, fid INTEGER NOT NULL)")) {
        ret = false;

        m_lastError = q.lastQuery();
    }


    return ret;
}

bool DBManager::connectDB()
{
    if( m_db.isOpen() )
        return true;

     m_db = QSqlDatabase::addDatabase( "QSQLITE" );
     m_db.setDatabaseName( m_dbPath );

    if ( !m_db.open() ) {
        QMessageBox::critical( 0, QApplication::tr( "Cannot connect to database" ),
                                    m_db.lastError().text() );
        qDebug() << "Cannot connect to database, SQL error: " << m_db.lastError().text();
        return false;
    }
    qDebug() << m_db.lastError().text();
    return true;
}

int DBManager::rssCount()
{
    QSqlQuery q;
    int res = 0;

    if( q.exec("SELECT COUNT(*) FROM rss_item") ) {
        q.next();
        res = qvariant_cast<int>(q.value(0));
    }else{
        m_lastError = q.lastError().text();
    }

    return res;
}

QList<NvRssItem *> DBManager::listRss(int start, int count)
{
    QList<NvRssItem*> res;
    QSqlQuery q;

    q.prepare("SELECT remote_id, title FROM rss_item LIMIT ? OFFSET ?") ;
    q.addBindValue( count );
    q.addBindValue( start );

    if( q.exec() ) {
        while( q.next() ) {
            int remote_id = q.value( 0 ).toInt();

            NvRssItem *item = new NvRssItem( remote_id, q.value( 1 ).toString() );
            res.append( item );
        }
    }else{
        m_lastError = q.lastError().text();
        qDebug() << "Cannot fetch local rss from" << start << " to " << (start  + count);
    }

    return res;
}

bool DBManager::storeRss(const NvRssItem *item)
{
    QSqlQuery q;
    q.prepare("INSERT INTO rss_item (remote_id, title) VALUES(?, ?)") ;
    q.addBindValue( item->id() );
    q.addBindValue( item->name() );

     if( !q.exec() ) {
         m_lastError = q.lastError().text();
         qDebug() << "Cannot store remote rss id: " << item->id();

         return false;
     }

     return true;
}

quint32 DBManager::maxNodeId() const
{
    QSqlQuery q;
    quint32 id = 0;

    if( q.exec("SELECT MAX(id) FROM " + NODES_TABLE) ) {
        q.next();

        id = q.value( 0 ).toUInt();
    }

    return id;
}

bool DBManager::loadFeedTree(NvFeedModel *model, NvFeedCategory *root)
{
    QSqlQuery q, fq;
    QList<FeedCategoryInternal> items;
    QMap<int, FeedCategoryInternal*> refs;

    q.prepare("SELECT id, title, parent FROM feed_category") ;

    if( q.exec() ) {
        while( q.next() ) {
            FeedCategoryInternal fd;
            fd.id = q.value( 0 ).toInt();
            fd.title = q.value( 1 ).toString();
            fd.parent = q.value( 2 ).toInt();
            fd.item = new NvFeedCategory( fd.id, fd.title );

            // load category feeds
            fq.prepare("SELECT fid FROM category_fids WHERE category_id = ?");
            fq.addBindValue( fd.id );
            if(fq.exec()) {
                while( fq.next() ) {
                    fd.item->addFeed( qvariant_cast<int>( fq.value(0)) );
                }
                fq.clear();
            }else{
                qDebug() << "Can load category feeds";
            }

            items << fd;
            refs[fd.id] = &items.last();
        }

        for(int i = 0; i < items.size(); ++i) {
            NvFeedCategory *croot = root;
            if(refs.contains(items[i].parent)) {
                croot = refs[ items[i].parent ]->item;
            }else{
                NvFeedCategory *cat = model->category( items[i].parent );
                if(cat)
                    croot = cat;
            }

            model->addCategory( items[i].item, croot);
        }
    }else{
        m_lastError = q.lastError().text();
        qDebug() << "Cannot fetch Feed tree " << m_lastError;
        return false;
    }

    return true;
}

bool DBManager::storeCategoryFids(int id, QList<int> fids)
{
   QSqlQuery q;
   q.prepare("DELETE FROM category_fids WHERE category_id = ?");
   q.addBindValue( id );
   if(q.exec()) {
       QListIterator<int> i(fids);

       while(i.hasNext()) {
           int fid = i.next();
           q.prepare("INSERT INTO category_fids (category_id, fid) VALUES(?, ?)");
           q.addBindValue( id );
           q.addBindValue( fid );
           if(!q.exec()) {
               m_lastError = q.lastError().text();
               qDebug() << "Cannot store Category fid ID " << m_lastError;
               return false;
           }
       }

       return true;
   }else{
        m_lastError = q.lastError().text();
       qDebug() << "Error while deleting category fids" << m_lastError;
   }

   return false;
}

void DBManager::clearFeeds()
{
    QSqlQuery q;
    if(!q.exec("DELETE FROM " + FEEDS_TABLE + " WHERE 1")) {
        m_lastError = q.lastError().text();
       qDebug() << "Error while deleting feeds from feeds table" << m_lastError;
    }
}

bool DBManager::addFeed(quint32 id, const QString &title)
{
    QSqlQuery q;
    q.prepare("INSERT INTO " + FEEDS_TABLE + " (id, title) VALUES(?, ?)");
    q.addBindValue( id );
    q.addBindValue( title );

    if(!q.exec()) {
        m_lastError = q.lastError().text();
       qDebug() << "Error while inserting feed to table" << m_lastError;
       return false;
    }

    return true;
}

int DBManager::storeFeedCategory(const QString& title, int parent_id, QList<int> fids, int id)
{
    QSqlQuery q;
    int current_id = 0;


    if(!id) {
        q.prepare("INSERT INTO feed_category (title, parent) VALUES(?, ?)");
        q.addBindValue( title );
        q.addBindValue( parent_id );
        if(q.exec()) {
            current_id = qvariant_cast<int>( q.lastInsertId() );
            if(!storeCategoryFids(current_id, fids))
                return 0;
        }else{
            m_lastError = q.lastError().text();
            qDebug() << "Cannot store Feed Category " << m_lastError;
            return 0;
        }
    }else{
        q.prepare("UPDATE feed_category SET title = ?, parent = ? WHERE id = ?");
        q.addBindValue( title );
        q.addBindValue( parent_id );
        q.addBindValue( id );

        if(!q.exec()) {
            m_lastError = q.lastError().text();
            qDebug() << "Cannot store Feed Category " << m_lastError;
            return 0;
        }

        current_id = id;

        if(!storeCategoryFids(current_id, fids))
            return 0;

    }

    return current_id;
}

void DBManager::setUpdateTime(DBManager::StoreTime type, QDateTime time)
{
    cached_updates[type]  = time;
    QSqlQuery q;

    q.prepare("UPDATE " + TIME_TABLE + " SET updated = ? WHERE type = ?");
    q.addBindValue( time.toTime_t() );
    q.addBindValue( type );
    if(!q.exec()) {
        m_lastError = q.lastError().text();
        qDebug() << "Cannot store updated time " << m_lastError;
    }else if( !q.numRowsAffected() ) {
        q.prepare("INSERT INTO " + TIME_TABLE + " (type, updated) VALUES(?, ?)");
        q.addBindValue( type );
        q.addBindValue( time.toTime_t() );

        if(!q.exec()) {
            m_lastError = q.lastError().text();
            qDebug() << "Cannot store updated time " << m_lastError;
        }
    }
}

QDateTime DBManager::getUpdateTime(DBManager::StoreTime type)
{
    if(cached_updates[type].isValid())
        return cached_updates[type];

     QSqlQuery q;
     q.prepare("SELECT updated FROM " + TIME_TABLE + " WHERE type = ?");
     q.addBindValue( type );
     if(q.exec()) {
         q.next();
         return QDateTime::fromTime_t(q.value(0).toUInt());
     }else{
         m_lastError = q.lastError().text();
         qDebug() << "Cannot get updated time " << m_lastError;
     }

     return QDateTime();
}

QSqlDatabase DBManager::connection()
{
    return m_db;
}
