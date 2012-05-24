#include "FilterWidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QListView>
#include <QMenu>
#include "NvFeedsTreeView.h"
#include "model/NvRssCachedModel.h"
#include "model/NvFeedModel.h"
#include "model/NvFeedItem.h"
#include "../dialogs/DateFilterDialog.h"
#include <QTextCodec>
#include <QDebug>

FilterWidget::FilterWidget(QListView *list, NvFeedModel* feeds, QWidget *parent) :
    QFrame(parent), _list(list), _feeds(feeds)
{
    setObjectName("filter");

    _searchEdit = new QLineEdit(this);
    _searchEdit->setObjectName("search-edit");
    _filterButton = new QPushButton(this);
    _filterButton->setIcon(QIcon(":/images/filter.png"));

    _model = dynamic_cast<NvRssCachedModel*>(list->model());
    _treeView = new NvFeedsTreeView(_model, this);
    _treeView->setVisible(false);
    _list->setParent(this);

    QPushButton *expandButton = new QPushButton(QIcon(":/images/check-dw.png"), "", this);
    expandButton->setCheckable(true);
    connect(expandButton, SIGNAL(toggled(bool)), _treeView, SLOT(setVisible(bool)));

    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(_filterButton);
    lay->addWidget(_searchEdit, 1);
    lay->addWidget(expandButton);

    // Init filter menu
    QMenu *contextMenu = new QMenu(this);
    contextMenu->addActions(createDateActions());
    contextMenu->addSeparator();
    contextMenu->addActions(createdFlagAction());
    actClear = new QAction(tr("Clear"), this);
    connect(actClear, SIGNAL(triggered()), this, SLOT(clearFilterClicked()));
    contextMenu->addAction(actClear);

    _filterButton->setMenu(contextMenu);
    connect(_filterButton, SIGNAL(clicked()), _filterButton, SLOT(showMenu()));
    connect(_treeView, SIGNAL(feedClicked(int)), this, SLOT(filterBySource(int)));
    connect(_searchEdit, SIGNAL(textChanged(QString)), this, SLOT(filterByTitle(QString)));
    clearFilter();

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->addLayout(lay);
    vbox->addWidget(_treeView);
    vbox->addWidget(_list, 1);
    setLayout(vbox);
}

QList<QAction*> FilterWidget::createDateActions()
{
    QList<QAction*> actions;

    actDay = new QAction(tr("Filter by Now"), this);
    connect(actDay, SIGNAL(triggered()), this, SLOT(filterByDateDay()));

    actYestarday = new QAction(tr("Filter by Yesterday"), this);
    connect(actYestarday, SIGNAL(triggered()), this, SLOT(filterByDateYesterday()));

    actWeek = new QAction(tr("Filter by Week"), this);
    connect(actWeek, SIGNAL(triggered()), this, SLOT(filterByDateWeek()));

    actCustom = new QAction(tr("Filter by Custom Date"), this);
    connect(actCustom, SIGNAL(triggered()), this, SLOT(filterByDateCustom()));

    actions << actDay << actYestarday << actWeek << actCustom;

    return actions;
}

QList<QAction*> FilterWidget::createdFlagAction()
{
    QList<QAction*> actions;
    QAction *a = new QAction(tr("Show Readed"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(filterByReaded()));
    actions << a;

    a = new QAction(tr("Show Unreaded"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(filterByUnreaded()));
    actions << a;

    return actions;
}

void FilterWidget::displaySourceFilter(const QString &title)
{

}

void FilterWidget::displayDateFilter(const QString &title)
{

}

void FilterWidget::clearFilter()
{
    _fid = 0;
    _startDate = _endDate = QDateTime();
    _showReaded = _showUnreaded = false;
    _query = "";
    filter();
}


void FilterWidget::filterBySource(int fid)
{
    NvFeedItem *feed = _feeds->feed( fid );
    if(!feed)
        return;
    _fid = fid;

    displaySourceFilter(feed->name());
    filter();
}

void FilterWidget::filter()
{
    QString filterString;
    bool bSearch = _query.length() > 3;

    if(_fid) {
        filterString += QString("feed_id = %1").arg(_fid);
    }
    \
    if(bSearch) {

        if(_fid) {
            filterString += " and ";
         }

        filterString += QString("UPPER(description) LIKE UPPER('%%1%')").arg(_query);
    }

    if(_startDate.isValid() && _endDate.isValid()) {
        if(_fid || bSearch) {
            filterString += " and ";
        }

        quint32 tStart = _startDate.toTime_t();
        quint32 tEnd =  _endDate.toTime_t();

        filterString += QString("created >= %1 and created <= %2").arg(tStart).arg(tEnd);
    }

    if(_showReaded || _showUnreaded) {

        if(_fid || (_startDate.isValid() && _endDate.isValid())) {
            filterString += " and ";
        }


        if(_showReaded && !_showUnreaded) {
             filterString += QString("readed > 0");
        }

        if(_showUnreaded && !_showReaded) {
            filterString += QString("readed is null or readed = 0");
        }
    }

   _model->setFilter(filterString);
   _model->select();
}

QString FilterWidget::formatDateString(const QDate start, const QDate end)
{
    QString str = start.toString(Qt::SystemLocaleShortDate);
    if(end.isValid())
        str += (QString(" - ") + end.toString(Qt::SystemLocaleShortDate));
    return str;
}

void FilterWidget::filterByDateDay()
{
    QDate day = QDate::currentDate();
    _startDate = QDateTime(day, QTime(0, 0, 0));
    _endDate = QDateTime(day.addDays(1), QTime(0, 0, 0));

    actDay->setChecked(true);
    actYestarday->setChecked(false);
    actWeek->setChecked(false);
    actCustom->setChecked(false);



     displayDateFilter(formatDateString(_startDate.date()));
     filter();
}

void FilterWidget::filterByDateWeek()
{
    QDate day = QDate::currentDate();
    _startDate = QDateTime(day.addDays(-7), QTime(0, 0, 0));
    _endDate = QDateTime(day, QTime(0, 0, 0));

    actDay->setChecked(false);
    actYestarday->setChecked(false);
    actWeek->setChecked(true);
    actCustom->setChecked(false);

    filter();
}

void FilterWidget::filterByDateYesterday()
{
    QDate day = QDate::currentDate();
    _startDate = QDateTime(day.addDays(-1), QTime(0, 0, 0));
    _endDate = QDateTime(day, QTime(0, 0, 0));

    actDay->setChecked(false);
    actYestarday->setChecked(true);
    actWeek->setChecked(false);
    actCustom->setChecked(false);


    filter();
}

void FilterWidget::filterByDateCustom()
{
    DateFilterDialog dlg(_startDate.date(), _endDate.date(), this);
    if(dlg.exec() == QDialog::Accepted) {
        _startDate = QDateTime(dlg.getStartDate(), QTime(0, 0, 0));
        _endDate = QDateTime(dlg.getEndDate(), QTime(0, 0, 0));

        actDay->setChecked(false);
        actYestarday->setChecked(false);
        actWeek->setChecked(false);
        actCustom->setChecked(true);

        filter();
    }
}

void FilterWidget::filterByReaded()
{
    _showReaded = true;
    filter();
}

void FilterWidget::filterByUnreaded()
{
    _showUnreaded = true;
    filter();
}

void FilterWidget::clearFilterClicked()
{
    actDay->setChecked(false);
    actYestarday->setChecked(false);
    actWeek->setChecked(false);
    actCustom->setChecked(false);

    clearFilter();
}

void FilterWidget::filterByTitle(QString query)
{
    _query = query;
    filter();
}
