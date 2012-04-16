#include "dbmanager.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

#define DATABASE_DEFAULT_PATH "local.db"

DBManager *DBManager::m_instance = NULL;

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

    if ( !QFile::exists( path ) ) {
        if ( !createDB() ) {
            QMessageBox::critical(0, QApplication::tr( "Cannot create database" ),
                                        QApplication::tr( m_db.lastError().text().toUtf8().data() ) );
            qDebug() << "Cannot create database, SQL error: " << m_db.lastError().text() << endl;
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
    QSqlQuery q;

    if ( !connectDB() )
       return false;

    // Rss table
    if(q.exec("CREATE TABLE rss_item (id INTEGER PRIMARY KEY, title TEXT NOT NULL, description TEXT, link TEXT)")) {
        ret = false;
        m_lastError = q.lastError().text();
    }

    return true;
}

bool DBManager::connectDB()
{
    if( m_db.isOpen() )
        return true;

     m_db = QSqlDatabase::addDatabase( "QSQLITE" );
     m_db.setDatabaseName( m_dbPath );

    if ( !m_db.open() ) {
        QMessageBox::critical( 0, QApplication::tr( "Cannot connect to database" ),
                                    QApplication::tr( m_db.lastError().text().toUtf8().data() ) );
        qDebug() << "Cannot connect to database, SQL error: " << m_db.lastError().text();
        return false;
    }
    return true;
}
