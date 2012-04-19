#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


class Connector;
class Node;
class CenterlaWidget;
class NvBaseListView;
class NvFeedsTreeView;

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
    void setupActions();

    void selectTids(QListWidget *widget, const QList<int> &tids);
    QList<int> getSelectedTids(QListWidget *widget);

    void arrangeItems(QWidget *parent, int t);
    void showNode(Node *node);
private: //widgets
    QListView *themesList;
    QLabel *messageLabel;
    QPushButton *btnSync, *btnNew;
    CenterlaWidget *view;
    NvBaseListView *rssList;
    NvFeedsTreeView *feedsTree;

private slots:
    void nodesLoaded();
    void rssLoaded();
    void createNode();
    void syncClicked();
    void rssItemSelected(QModelIndex index);

    void loadNode(QModelIndex index);
    void nodeLoaded(Node *n);
    void attachRss(QModelIndex index);
    void networkError(QString msg);
    void actionLogin(QString userLogin, QString userPassword);

    // actions
    void setListViewMode();
    void setFullViewMode();
};

#endif // MAINWINDOW_H
