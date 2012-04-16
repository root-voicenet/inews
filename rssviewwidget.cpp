#include "rssviewwidget.h"
#include "rssitem.h"
#include "taxonomywidget.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include <QtGui>
#include <QWebView>

RssViewWidget::RssViewWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUI();
}

void RssViewWidget::setupUI()
{
    QTabWidget *tabs = new QTabWidget(this);
    tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *pageOverwiev = new QWidget(tabs);
    tabs->addTab(pageOverwiev, tr("Overwiev"));

    QWidget *pageBrowser = new QWidget(tabs);
    m_browser = new QWebView(pageBrowser);
    m_browser->setMaximumWidth(400);
    tabs->addTab(pageBrowser, tr("Browser"));

    // set overwiev page layout
    titleLabel = new QLabel(pageOverwiev);
    titleLabel->setWordWrap(true);
    textLabel = new QLabel(pageOverwiev);
    textLabel->setWordWrap(true);
    taxonomy = new TaxonomyWidget(pageOverwiev);


    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel(tr("Title"), pageOverwiev), 0, 0, 1, 1);
    gridLayout->addWidget(titleLabel, 0, 1, 1, 2);
    gridLayout->addWidget(new QLabel(tr("Description"), pageOverwiev), 1, 0, 1, 1);
    gridLayout->addWidget(textLabel, 1, 1, 1, 2);
    gridLayout->setRowStretch(1, 1);
    pageOverwiev->setLayout(gridLayout);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(tabs);
    vbox->addWidget(taxonomy);
    setLayout(vbox);

    vbox = new QVBoxLayout;
    vbox->addWidget(m_browser);
    pageBrowser->setLayout(vbox);
    setMaximumWidth(400);
}

void RssViewWidget::loadRss(RssItem *rss)
{
    taxonomy->clearSelection();
    if(!rss->getTids().isEmpty()) {
        taxonomy->selectTaxonomy(rss->getTids());
    }
    titleLabel->setText(rss->getTitle());
    textLabel->setText(rss->getDescription());

    QUrl url(rss->getLink());
    if(url.isValid()) {
        m_browser->setUrl(url);
    }




}

void RssViewWidget::updateTaxonomy()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    QTreeWidgetItem* tax = rm->getTaxonomy();
    if(tax->childCount() > 0) {
        taxonomy->loadTaxonomy(tax);
    }
}

bool RssViewWidget::storeRss(RssItem *rss)
{
    QList<TaxonomyTerm*> tids = taxonomy->selectedTaxonomy();
    rss->setTids(tids);

    return true;
}
