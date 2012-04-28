#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


class MainWindow;
class MediaWindow;
class Connector;

class WindowManager
{
public:
    ~WindowManager();
    static WindowManager *instance();

    MainWindow *mainWindow();
    MediaWindow *mediaWindow();
    Connector *connector() { return m_connector; }

    void showMainWindow();
    void showMediaWindow();
    void init();
private:
    WindowManager();

    static WindowManager *_instance;
    MainWindow *m_main;
    MediaWindow *m_media;
    Connector* m_connector;
};

#endif // WINDOWMANAGER_H
