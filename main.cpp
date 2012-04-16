#include "newsapplication.h"
#include "mainwindow.h"

#include "model/nvrssitem.h"
#include "model/NvRssCachedModel.h".h"
#include "view/NvBaseListView.h"
#include "dbmanager.h"
#include <QNetworkAccessManager>
#include <QDebug>

int main(int argc, char *argv[])
{
    /*
    NewsApplication a(argc, argv);
    MainWindow w;
    w.show();
    */

    QApplication a(argc, argv);
    NvRssCachedModel model(0);
    NvBaseListView w(0);
    QNetworkAccessManager manager;
    DBManager *dbman;

    // First init Database manager
    dbman = DBManager::instance();

    if(!dbman->init( a.applicationDirPath() + "/local4.db" )) {
        return 0;
    }

    w.setModel(&model);
    w.expandAll();
    w.show();

    return a.exec();
}
