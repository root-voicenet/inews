#include "taxonomywidget.h"
#include "taxonomyterm.h"
#include <QtGui>

TaxonomyWidget::TaxonomyWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUI();
}

void TaxonomyWidget::setupUI()
{
    themeList = new QListWidget(this);
    geoList = new QListWidget(this);
    setMaximumSize(220, 1000000);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(themeList);
    themeList->setMaximumSize(100, 200);
    themeList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    geoList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    geoList->setMaximumSize(100, 200);
    hbox->addWidget(geoList);

    setLayout(hbox);
}

void TaxonomyWidget::loadTaxonomy(QListWidget *widget, const QList<TaxonomyTerm *> &terms)
{
    widget->clear();
    for(int i = 0; i < terms.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(terms[i]->getTitle());
        QVariant data((int)terms[i]);
        item->setData(Qt::UserRole + 1, data);
        item->setCheckState(Qt::Unchecked);

        widget->addItem(item);
    }
}

void TaxonomyWidget::loadThemeTaxonomy(const QList<TaxonomyTerm *> &terms)
{
    loadTaxonomy(themeList, terms);
}

void TaxonomyWidget::loadGeoTaxonomy(const QList<TaxonomyTerm *> &terms)
{
    loadTaxonomy(geoList, terms);
}

void TaxonomyWidget::clearSelection()
{
    clearSelection(themeList);
    clearSelection(geoList);
}

void TaxonomyWidget::clearSelection(QListWidget *widget)
{
    for(int i = 0; i < widget->count(); ++i) {
        widget->item(i)->setCheckState(Qt::Unchecked);
    }
}

void TaxonomyWidget::selectTaxonomy(const QList<TaxonomyTerm *> &selected)
{
    selectTaxonomy(themeList, selected);
    selectTaxonomy(geoList, selected);
}

void TaxonomyWidget::selectTaxonomy(QListWidget *widget, const QList<TaxonomyTerm *> &terms)
{
    QList<int> tids;

    for(int i = 0; i < terms.size(); ++i)
        tids << terms[i]->getId();

    for(int i = 0; i < widget->count(); ++i) {
        QListWidgetItem *item = widget->item(i);
        TaxonomyTerm* term = reinterpret_cast<TaxonomyTerm*>(item->data(Qt::UserRole + 1).toInt());
        if(tids.indexOf(term->getId()) != -1) {
            item->setCheckState(Qt::Checked);
        }else
            item->setCheckState(Qt::Unchecked);
    }
}

QList<TaxonomyTerm*> TaxonomyWidget::selectedTaxonomy(QListWidget *widget)
{
    QList<TaxonomyTerm*> res;
    for(int i = 0; i < widget->count(); ++i) {
        QListWidgetItem *item = widget->item(i);

        if(item && item->checkState() == Qt::Checked) {
            TaxonomyTerm* term = reinterpret_cast<TaxonomyTerm*>(item->data(Qt::UserRole + 1).toInt());
            if(term) {
                 res.append(term);
            }

        }
    }

    return res;
}

QList<TaxonomyTerm*> TaxonomyWidget::selectedTaxonomy()
{
    QList<TaxonomyTerm*> res;
    res << selectedTaxonomy(themeList);
    res << selectedTaxonomy(geoList);

    return res;
}



