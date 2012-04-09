#include "newsapplication.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    NewsApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
