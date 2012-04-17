#include "mainwindow.h"
#include "model/nvrssitem.h"
#include "model/NvRssCachedModel.h"
#include "view/NvBaseListView.h"
#include "dbmanager.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // First init Database manager
    DBManager *dbman = DBManager::instance();

    if(!dbman->init( a.applicationDirPath() + "/local4.db" )) {
        return 0;
    }

    MainWindow w;
    w.show();


    return a.exec();
}
