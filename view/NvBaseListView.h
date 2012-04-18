#ifndef NVOBJECTLISTVIEW_H
#define NVOBJECTLISTVIEW_H

#include <QListView>
#include <QPointer>

class NvObjectModel;
class NvBaseItemDelegate;
class NvLineItemDelegate;

class NvBaseListView : public QListView
{
    Q_OBJECT
public:
    explicit NvBaseListView(QWidget *parent = 0);
    ~NvBaseListView();

    enum {
        VIEW_FULL = 0,
        VIEW_LINE
    };

    void setModel(QAbstractItemModel *model);

    void setViewMode( int mode = VIEW_FULL);
private:
    QPointer<NvObjectModel> model_;
    QPointer<NvBaseItemDelegate> _baseDel;
    //QPointer<NvLineItemDelegate> _lineDel;
signals:

public slots:

};

#endif // NVOBJECTLISTVIEW_H
