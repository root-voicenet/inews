#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>


class Node;
class CenterlaWidget;
class NvBaseListView;
class NvFeedsTreeView;
class NvSortFilterModel;

QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QDockWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QSplitter)

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void showError(const QString& str);
private:
    // initialize widgets
    void initWidgets();
    void setupUI();
    void setupActions();
    void initStatusBar();
    void initToolbar();

    void selectTids(QListWidget *widget, const QList<int> &tids);
    QList<int> getSelectedTids(QListWidget *widget);

    void arrangeItems(QWidget *parent, int t);
    void showNode(Node *node);
private: //widgets
    QListView *themesList;
    QLabel *messageLabel;
    QAction *syncAction, *galleryAction, *themeAction;
    CenterlaWidget *view;
    NvBaseListView *rssList;
    QPushButton *leftSideBtn, *rightSideBtn;
    QSplitter* left;
    QWidget *right;

private slots:
    void nodesLoaded();
    void rssLoaded();
    void createNode();
    void syncClicked();
    void itemClicked(QModelIndex index);
    void itemDoubleClicked(QModelIndex index);

    void loadNode(QModelIndex index);
    void nodeLoaded(Node *n);
    void networkError(QString msg);
    void actionLogin(QString userLogin, QString userPassword);
    void userLoged();
    void showMedia();
    void collapseLeftPane();
    void collapseRightPane();

    // actions
    void setListViewMode();
    void setFullViewMode();
};

#endif // MAINWINDOW_H
