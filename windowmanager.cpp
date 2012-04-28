#include "windowmanager.h"
#include "mainwindow.h"
#include "mediawindow.h"
#include "connector.h"

WindowManager *WindowManager::_instance = NULL;

WindowManager::WindowManager()
    : m_main(0), m_media(0), m_connector(0)
{

}

void WindowManager::init()
{
    m_connector = new Connector("http://test.irkipedia.ru/news/api");
    m_main = new MainWindow();
    m_media = new MediaWindow(this);

}

WindowManager::~WindowManager()
{
    delete m_main;
    delete m_media;
    delete m_connector;
}

WindowManager *WindowManager::instance()
{
    if(!_instance)
        _instance = new WindowManager;
    return _instance;
}

MainWindow *WindowManager::mainWindow()
{
    return m_main;
}

MediaWindow *WindowManager::mediaWindow()
{
    return m_media;
}

void WindowManager::showMainWindow()
{
    if(!m_main->isVisible())
        m_main->show();
}

void WindowManager::showMediaWindow()
{
    if(!m_media->isVisible())
        m_media->show();
}


