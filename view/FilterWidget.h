#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QFrame>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QAction)

class NvFeedsTreeView;
class NvFeedModel;
class NvRssCachedModel;
class FilterWidget : public QFrame
{
    Q_OBJECT
private:
    QLineEdit *_searchEdit;
    QPushButton *_filterButton;
    QListView *_list;
    NvFeedsTreeView *_treeView;
    NvRssCachedModel *_model;
    NvFeedModel *_feeds;
    QDateTime _startDate, _endDate;
    QAction *actDay, *actYestarday, *actWeek, *actCustom, *actClear;
    QString _query;
    int _fid;
    bool _showReaded;
    bool _showUnreaded;



    void clearFilter();
    void displaySourceFilter(const QString& title);
    void displayDateFilter(const QString& title);
    void filter();
    QString formatDateString(const QDate start, const QDate end = QDate());
    QList<QAction*> createDateActions();
    QList<QAction*> createdFlagAction();

public:
    explicit FilterWidget(QListView *source, NvFeedModel *feeds, QWidget *parent);
        

signals:
    
public slots:
    void clearFilterClicked();
    void filterBySource(int fid);
    void filterByTitle(QString query);
    void filterByDateDay();
    void filterByDateYesterday();
    void filterByDateWeek();
    void filterByDateCustom();
    void filterByReaded();
    void filterByUnreaded();
};

#endif // FILTERWIDGET_H
