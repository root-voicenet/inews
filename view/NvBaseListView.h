#ifndef NVOBJECTLISTVIEW_H
#define NVOBJECTLISTVIEW_H

#include <QListView>
#include <QPointer>

class NvObjectModel;
class NvRssItemDelegate;
class NvLineItemDelegate;
class NvRssItem;
class QAction;

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
    QPointer<NvRssItemDelegate> _baseDel;
    QAction* m_attachAction;
    //QPointer<NvLineItemDelegate> _lineDel;
signals:
    void attachSelected(quint32 rss_id);

private slots:
     void showContextMenu(QPoint point);
     void attachRss();
public slots:

};

#endif // NVOBJECTLISTVIEW_H
