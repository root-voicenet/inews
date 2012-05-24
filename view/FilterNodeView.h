#ifndef FILTERNODEVIEW_H
#define FILTERNODEVIEW_H

#include <QFrame>
#include <QDateTime>

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QListView)
QT_FORWARD_DECLARE_CLASS(QAction)

class NvNodeModel;
class FilterNodeWiew : public QFrame
{
    Q_OBJECT
private:
    QLineEdit *_searchEdit;
    QPushButton *_filterButton;
    QListView *_list;
    NvNodeModel *_model;
    QDateTime _startDate, _endDate;
    QAction *actDay, *actYestarday, *actWeek, *actCustom, *actClear;
    QString _query;


    void clearFilter();
    void displaySourceFilter(const QString& title);
    void displayDateFilter(const QString& title);
    void filter();
    QString formatDateString(const QDate start, const QDate end = QDate());
    QList<QAction*> createDateActions();
    QList<QAction*> createSortActions();

public:
    explicit FilterNodeWiew(QListView *source, QWidget *parent);
        

signals:
    
public slots:
    void clearFilterClicked();
    void filterByTitle(QString query);
    void filterByDateDay();
    void filterByDateYesterday();
    void filterByDateWeek();
    void filterByDateCustom();

    void sortByDateAsc();
    void sortByDateDsc();
    void sortByTitleAsc();
    void sortByTitleDsc();
};

#endif // FILTERNODEVIEW_H
