#include "newsapplication.h"
#include "resourcemanager.h"

NewsApplication::NewsApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
    m_resourcemanager = new ResourceManager;
}

ResourceManager* NewsApplication::getRM() {
    return m_resourcemanager;
}

NewsApplication::~NewsApplication()
{
    delete m_resourcemanager;
}
