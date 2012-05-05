#ifndef NVFEEDSTREEVIEW_H
#define NVFEEDSTREEVIEW_H

#include <QModelIndex>
#include <QWidget>

class NvFeedModel;
class NvFeedCategory;

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QTreeView)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class NvFeedsTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit NvFeedsTreeView(QWidget *parent = 0);
    QAction* createFeedAction() { return m_addFeedAction; }

     static QString askCategoryName(QWidget *parent, const QString &title = QString());
private:
    void init();

private:
    NvFeedModel *m_model;
    QTreeView *m_tree;
    QAction *m_addAction, *m_renAction, *m_delAction, *m_addFeedAction, *m_delFeedAction;
    QPushButton *m_btnClear;
signals:
    void feedClicked(int fid);
private slots:
    void addCategory();
    void renameCategory();
    void deleteCategory();
    void addFeed();
    void delFeed();
    void itemClicked(QModelIndex index);
    void showContextMenu(QPoint point);
    void clearFilter();
};

#endif // NVFEEDSTREEVIEW_H
