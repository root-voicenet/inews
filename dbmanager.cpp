#include "dbmanager.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QVariant>
#include "model/NvFeedCategory.h"

#define DATABASE_DEFAULT_PATH "local.db"

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
    if(!q.exec("CREATE TABLE rss_item (id INTEGER PRIMARY KEY, remote_id INTEGER, title TEXT NOT NULL)")) {
        ret = false;

        m_lastError = q.lastQuery();
    }

    if(!q.exec("CREATE TABLE feed_category (id INTEGER PRIMARY KEY, title TEXT NOT NULL, parent INTEGER)")) {
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

QList<NvLocalRssItem*> DBManager::listRss(int start, int count)
{
    QList<NvLocalRssItem*> res;
    QSqlQuery q;

    q.prepare("SELECT remote_id, title FROM rss_item LIMIT ? OFFSET ?") ;
    q.addBindValue( count );
    q.addBindValue( start );

    if( q.exec() ) {
        while( q.next() ) {
            int remote_id = q.value( 0 ).toInt();

            NvLocalRssItem *item = new NvLocalRssItem( remote_id, q.value( 1 ).toString() );
            res.append( item );
        }
    }else{
        m_lastError = q.lastError().text();
        qDebug() << "Cannot fetch local rss from" << start << " to " << (start  + count);
    }

    return res;
}

bool DBManager::storeRss(const NvRemoteRssItem *item)
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

bool DBManager::loadFeedTree(NvFeedCategory *root)
{
    QSqlQuery q;
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

            items << fd;
            refs[fd.id] = &items.last();
        }

        for(int i = 0; i < items.size(); ++i) {
            NvFeedCategory *croot = root;
            if(items[i].parent && refs.contains(items[i].parent)) {
                croot = refs[ items[i].parent ]->item;
            }

            croot->appendChild( items[i].item );
        }
    }else{
        m_lastError = q.lastError().text();
        qDebug() << "Cannot fetch Feed tree " << m_lastError;
        return false;
    }

    return true;
}

bool DBManager::storeFeedCategory(NvFeedCategory *cat, bool update)
{
    QSqlQuery q;
    int id = 0;

    int parent_id = 0;

    if(cat->parent()) {
        parent_id = qobject_cast<NvFeedCategory*>(cat->parent())->id();
    }

    if(!update) {

        q.prepare("INSERT INTO feed_category (title, parent) VALUES(?, ?)");
        q.addBindValue( cat->title() );
        q.addBindValue( parent_id );
        if(q.exec()) {
            id = qvariant_cast<int>( q.lastInsertId() );
            cat->setId( id );
        }else{
            m_lastError = q.lastError().text();
            qDebug() << "Cannot store Feed Category " << m_lastError;
            return false;
        }
    }else{
        q.prepare("UPDATE feed_category SET title = ?, parent = ? WHERE id = ?");
        q.addBindValue( cat->title() );
        q.addBindValue( parent_id );
        q.addBindValue( cat->id() );

        if(!q.exec()) {
            m_lastError = q.lastError().text();
            qDebug() << "Cannot store Feed Category " << m_lastError;
            return false;
        }
    }

    return true;
}
