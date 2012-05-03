#ifndef MEDIAWINDOW_H
#define MEDIAWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QListWidget>
#include <QFuture>
#include <QTreeWidgetItem>

#include "addiconimagethread.h"
#include "model/NvMediaModel.h"

namespace Ui {
    class MediaWindow;
}

class Node;
class WindowManager;
class QPushButton;

class MediaWindow : public QMainWindow {
    Q_OBJECT
public:
    MediaWindow(WindowManager* wm, QWidget *parent = 0);
    ~MediaWindow();

    bool parseRemoteFiles(QVariant *result);
    void selectFile(Node *n);
    NvMediaModel *getModel();
protected:
    void changeEvent(QEvent *e);
     void closeEvent(QCloseEvent *event);
     void showEvent (QShowEvent * event);
     void hideEvent(QHideEvent *event);
signals:
     void fileSelected();

private slots:
    //void treeWidget_itemExpanded(QTreeWidgetItem *item);
    //void treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void listView_itemDoubleClicked(QModelIndex index);
    void tabMain_tabCloseRequested(int index_tab);
    void tabMain_currentChanged(int index_tab);
    void addFileClicked();
    void btnUpload_clicked();
    void btnRefresh_clicked();
private:
    Ui::MediaWindow *ui;   

    void ShowImageIconFile(QString dirPath);
    void AddItemToTreeWidgetFileSystem();
    QFileInfoList getListFiles(QString dirPath) const;

private:
    NvMediaModel model;
    WindowManager *m_wm;
    Node *currentNode;
};


#endif // MEDIAWINDOW_H
