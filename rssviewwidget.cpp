#include "rssviewwidget.h"
#include "rssitem.h"
#include "taxonomywidget.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include <QGridLayout>
#include <QLabel>

RssViewWidget::RssViewWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUI();
}

void RssViewWidget::setupUI()
{
    titleLabel = new QLabel(this);
    textLabel = new QLabel(this);
    taxonomy = new TaxonomyWidget(this);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(new QLabel(tr("Title"), this), 0, 0, 1, 1);
    gridLayout->addWidget(titleLabel, 0, 1, 1, 2);
    gridLayout->addWidget(new QLabel(tr("Description"), this), 1, 0, 1, 1);
    gridLayout->addWidget(textLabel, 1, 1, 1, 2);
    gridLayout->addWidget(taxonomy, 3, 0, 1, 2);

    setLayout(gridLayout);
}

void RssViewWidget::loadRss(RssItem *rss)
{
    taxonomy->clearSelection();
    if(!rss->getTids().isEmpty()) {
        taxonomy->selectTaxonomy(rss->getTids());
    }
    titleLabel->setText(rss->getTitle());
    textLabel->setText(tr("Dummy"));
}

void RssViewWidget::updateTaxonomy()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    QList<TaxonomyTerm*> themeList = rm->getTaxonomy(ResourceManager::TAXONOMY_THEME);
    if(themeList.size() > 0) {
        taxonomy->loadThemeTaxonomy(themeList);
    }

    QList<TaxonomyTerm*> geoList = rm->getTaxonomy(ResourceManager::TAXONOMY_GEO);
    if(geoList.size() > 0) {
        taxonomy->loadGeoTaxonomy(geoList);
    }
}

bool RssViewWidget::storeRss(RssItem *rss)
{
    QList<TaxonomyTerm*> tids = taxonomy->selectedTaxonomy();
    rss->setTids(tids);

    return true;
}
