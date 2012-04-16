#ifndef NVOBJECTLISTVIEW_H
#define NVOBJECTLISTVIEW_H

#include <QTreeView>
#include <QPointer>

class NvObjectModel;
class NvBaseItemDelegate;

class NvBaseListView : public QTreeView
{
    Q_OBJECT
public:
    explicit NvBaseListView(QWidget *parent = 0);
    ~NvBaseListView();

    void setModel(QAbstractItemModel *model);

private:
    QPointer<NvObjectModel> model_;
    QPointer<NvBaseItemDelegate> delegate_;
signals:

public slots:

};

#endif // NVOBJECTLISTVIEW_H
