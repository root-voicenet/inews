#ifndef NEWSAPPLICATION_H
#define NEWSAPPLICATION_H

#include <QApplication>

class ResourceManager;
class NewsApplication : public QApplication
{
    Q_OBJECT
public:
    explicit NewsApplication(int &argc, char **argv);
    ~NewsApplication();

    ResourceManager *getRM();

private:
    ResourceManager *m_resourcemanager;

signals:
    
public slots:
    
};

#endif // NEWSAPPLICATION_H
