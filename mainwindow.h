#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


class Connector;
class TextEdit;
class Node;

QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QDockWidget)

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Connector* m_connector;

    // initialize widgets
    void initWidgets();
    void setupUI();
    void setupDockablePanels();

    void selectTids(QListWidget *widget, const QList<int> &tids);
    QList<int> getSelectedTids(QListWidget *widget);

    void arrangeItems(QWidget *parent, int t);
    void showNode(Node *node);

    Node* currentNode;
private: //widgets
    QListView *rssList, *themesList;
    QListWidget *taxThemeList, *taxGeoList, *attachedRssList;
    TextEdit *textEdit;
    QLineEdit *titleEdit;
    QDockWidget *dock;
private slots:
    void taxonomyLoaded();
    void saveNode();
    void nodesLoaded();
    void rssLoaded();
    void syncClicked();
    void rssItemSelected(QModelIndex index);
    void dockLocationChanged(Qt::DockWidgetArea area);
    void loadNode(QModelIndex index);
    void nodeLoaded(Node *n);
    void attachRss(QModelIndex index);
};

#endif // MAINWINDOW_H
