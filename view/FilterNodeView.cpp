#include "FilterNodeView.h"
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QListView>
#include <QMenu>
#include "model/NvNodeModel.h"
#include "../dialogs/DateFilterDialog.h"

FilterNodeWiew::FilterNodeWiew(QListView *list, QWidget *parent) :
    QFrame(parent), _list(list)
{
    setObjectName("filter");

    _searchEdit = new QLineEdit(this);
    _searchEdit->setObjectName("search-edit");
    _filterButton = new QPushButton(this);
    _filterButton->setIcon(QIcon(":/images/filter.png"));

    _model = dynamic_cast<NvNodeModel*>(list->model());
    _list->setParent(this);

    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(_filterButton);
    lay->addWidget(_searchEdit, 1);

    // Init filter menu
    QMenu *contextMenu = new QMenu(this);
    contextMenu->addActions(createDateActions());
    contextMenu->addSeparator();
    contextMenu->addActions(createSortActions());
    actClear = new QAction(tr("Clear"), this);
    connect(actClear, SIGNAL(triggered()), this, SLOT(clearFilterClicked()));
    contextMenu->addAction(actClear);

    _filterButton->setMenu(contextMenu);
    connect(_filterButton, SIGNAL(clicked()), _filterButton, SLOT(showMenu()));
    connect(_searchEdit, SIGNAL(textChanged(QString)), this, SLOT(filterByTitle(QString)));

    clearFilter();

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->addLayout(lay);
    vbox->addWidget(_list, 1);
    setLayout(vbox);
}

QList<QAction*> FilterNodeWiew::createDateActions()
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

QList<QAction*> FilterNodeWiew::createSortActions()
{
    QList<QAction*> actions;

    QAction *a = new QAction(tr("Sort by Date Asc"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(sortByDateAsc()));
    actions << a;

    a = new QAction(tr("Sort by Date Dsc"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(sortByDateDsc()));
    actions << a;

    a = new QAction(tr("Sort by Title Asc"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(sortByTitleAsc()));
    actions << a;

    a = new QAction(tr("Sort by Title Asc"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(sortByTitleDsc()));
    actions << a;

    return actions;
}


void FilterNodeWiew::displaySourceFilter(const QString &title)
{

}

void FilterNodeWiew::displayDateFilter(const QString &title)
{

}

void FilterNodeWiew::clearFilter()
{
    _startDate = _endDate = QDateTime();
    _query = "";
    filter();
}

void FilterNodeWiew::filter()
{
    QString filterString;
    bool bSearch = _query.length() > 3;

          if(bSearch) {

        filterString += QString("title LIKE ÌÒÑ-").arg(_query);
    }

    if(_startDate.isValid() && _endDate.isValid()) {
        if(bSearch) {
            filterString += " and ";
        }

        quint32 tStart = _startDate.toTime_t();
        quint32 tEnd =  _endDate.toTime_t();

        filterString += QString("created >= %1 and created <= %2").arg(tStart).arg(tEnd);
    }

   _model->setFilter(filterString);
   _model->select();
}

QString FilterNodeWiew::formatDateString(const QDate start, const QDate end)
{
    QString str = start.toString(Qt::SystemLocaleShortDate);
    if(end.isValid())
        str += (QString(" - ") + end.toString(Qt::SystemLocaleShortDate));
    return str;
}

void FilterNodeWiew::filterByDateDay()
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

void FilterNodeWiew::filterByDateWeek()
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

void FilterNodeWiew::filterByDateYesterday()
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

void FilterNodeWiew::filterByDateCustom()
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


void FilterNodeWiew::clearFilterClicked()
{
    actDay->setChecked(false);
    actYestarday->setChecked(false);
    actWeek->setChecked(false);
    actCustom->setChecked(false);

    clearFilter();
}

void FilterNodeWiew::filterByTitle(QString query)
{
    _query = query;
    filter();
}

void FilterNodeWiew::sortByDateAsc()
{
    _model->setSortMode(NvNodeModel::SORT_DATE_ASC);
    _model->select();
}

void FilterNodeWiew::sortByDateDsc()
{
    _model->setSortMode(NvNodeModel::SORT_DATE_DSC);
    _model->select();
}

void FilterNodeWiew::sortByTitleAsc()
{
    _model->setSortMode(NvNodeModel::SORT_TITLE_ASC);
    _model->select();
}

void FilterNodeWiew::sortByTitleDsc()
{
    _model->setSortMode(NvNodeModel::SORT_TITLE_DSC);
    _model->select();
}
