#include "windowmanager.h"
#include "model/nvrssitem.h"
#include "model/NvRssCachedModel.h"
#include "view/NvBaseListView.h"
#include "dbmanager.h"
#include "view/TagsWidget.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath(a.applicationDirPath() + "/plugins");

    QTranslator translator;
    if(translator.load(a.applicationDirPath() + "/inews_ru")) {
        a.installTranslator(&translator);
    }

    QFile stylesheet(a.applicationDirPath() + "/style.txt");
    if(stylesheet.exists() && stylesheet.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(stylesheet.readAll());
    }

    // First init Database manager
    DBManager *dbman = DBManager::instance();

    if(!dbman->init( QDir::homePath() + "/local5.db" )) {
        return 0;
    }

    WindowManager *wm = WindowManager::instance();
    wm->init();
    wm->showMainWindow();

    return a.exec();
}
