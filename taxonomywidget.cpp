#include "taxonomywidget.h"
#include "taxonomyterm.h"
#include <QStandardItem>
#include <QtGui>

TaxonomyWidget::TaxonomyWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUI();
}

void TaxonomyWidget::setupUI()
{
    taxonomyList = new QTreeWidget(this);
    taxonomyList->setMinimumSize(150, 200);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->addWidget(taxonomyList);
    setLayout(vbox);

    connect(taxonomyList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SIGNAL(selected()));
}

void TaxonomyWidget::loadTaxonomy(QTreeWidgetItem *root)
{
    if(root) {
        taxonomyList->clear();
        QTreeWidgetItem* treeRoot = taxonomyList->invisibleRootItem();

        for(int i = 0; i < root->childCount(); ++i ) {
            treeRoot->addChild( root->child(i)->clone() );
        }
    }
}


void TaxonomyWidget::clearSelection()
{
    QList<int> tids;
    selectItem(tids);
}

void TaxonomyWidget::selectItem(const QList<int> &tids, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = parent ? parent : taxonomyList->invisibleRootItem();
    if(item) {
        TaxonomyTerm* term = reinterpret_cast<TaxonomyTerm*>(item->data(0, Qt::UserRole + 1).toInt());
        if(term) {
            if(tids.indexOf(term->getId()) != -1) {
                item->setCheckState(0, Qt::Checked);
            }else
                item->setCheckState(0, Qt::Unchecked);
        }

        if(item->childCount() > 0 ) {
            for(int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem *subitem = item->child(j);
                selectItem(tids, subitem);
            }
        }
    }
}

void TaxonomyWidget::selectTaxonomy(const QList<int> &terms)
{
     selectItem(terms);
}

QList<int> TaxonomyWidget::selectedItem(QTreeWidgetItem *parent)
{
    QList<int> res;

    QTreeWidgetItem *item = parent ? parent : taxonomyList->invisibleRootItem();
    if(item) {
        TaxonomyTerm* term = reinterpret_cast<TaxonomyTerm*>(item->data(0, Qt::UserRole + 1).toInt());
        if(term && item->checkState(0) == Qt::Checked) {
            res << term->getId();
        }

        if(item->childCount() > 0) {
            for(int j = 0; j < item->childCount(); ++j) {
                QTreeWidgetItem *subitem = item->child(j);
                res << selectedItem(subitem);
            }
        }
    }

    return res;
}

QList<int> TaxonomyWidget::selectedTaxonomy()
{
    return selectedItem();
}


