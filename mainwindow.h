#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


class Connector;
class Node;
class CenterlaWidget;

QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QDockWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void showError(const QString& str);
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
private: //widgets
    QListView *rssList, *themesList;
    QLabel *messageLabel;
    QPushButton *btnSync, *btnNew;
    QDockWidget *dock;
    CenterlaWidget *view;

private slots:
    void nodesLoaded();
    void rssLoaded();
    void createNode();
    void syncClicked();
    void rssItemSelected(QModelIndex index);
    void dockLocationChanged(Qt::DockWidgetArea area);
    void loadNode(QModelIndex index);
    void nodeLoaded(Node *n);
    void attachRss(QModelIndex index);
    void networkError(QString msg);
    void actionLogin(QString userLogin, QString userPassword);

};

#endif // MAINWINDOW_H
