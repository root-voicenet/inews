#ifndef NVFEEDSTREEVIEW_H
#define NVFEEDSTREEVIEW_H

#include <QTreeView>

class NvFeedModel;
class NvFeedCategory;

QT_FORWARD_DECLARE_CLASS(QAction)
class NvFeedsTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit NvFeedsTreeView(QWidget *parent = 0);
    

private:
    void init();
    bool askCategoryName(NvFeedCategory *item);
private:
    NvFeedModel *m_model;
    QAction *m_addAction, *m_renAction, *m_delAction;
signals:
    
private slots:
    void addCategory();
    void renameCategory();
    void deleteCategory();

    void showContextMenu(QPoint point);
};

#endif // NVFEEDSTREEVIEW_H
