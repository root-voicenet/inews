#include "rssviewwidget.h"
#include "model/nvrssitem.h"
#include "taxonomywidget.h"
#include "resourcemanager.h"
#include <QtGui>
#include <QWebView>

RssViewWidget::RssViewWidget(QWidget *parent) :
    QWidget(parent), m_current(NULL)
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
    tabs->addTab(pageBrowser, tr("Browser"));

    // set overwiev page layout
    titleLabel = new QLabel(pageOverwiev);
    titleLabel->setWordWrap(true);
    textLabel = new QLabel(pageOverwiev);
    QFont font = QApplication::font();
    font.setBold( true );
    titleLabel->setFont(font);
    textLabel->setWordWrap(true);
    taxonomy = new TaxonomyWidget(this);

    attachLink = new QLabel("<a href=\"#\">Attach</a>", this);
    attachLink->hide();
    connect(attachLink, SIGNAL(linkActivated(QString)), this, SIGNAL(attachClicked()));


    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel(tr("Title"), pageOverwiev), 0, 0, 1, 1);
    gridLayout->addWidget(titleLabel, 0, 1, 1, 2);
    gridLayout->addWidget(new QLabel(tr("Description"), pageOverwiev), 1, 0, 1, 1, Qt::AlignTop);
    gridLayout->addWidget(textLabel, 1, 1, 1, 2, Qt::AlignTop);
    gridLayout->setRowStretch(1, 1);
    gridLayout->setColumnStretch(1, 1);
    pageOverwiev->setLayout(gridLayout);

    QGridLayout *grid = new QGridLayout;
    grid->setMargin(0);
    grid->addWidget(tabs, 0, 0, 1, 2, Qt::AlignTop);
    grid->addWidget(attachLink, 1, 1, 1, 1);
    grid->addWidget(taxonomy, 2, 0, 1, 2);
    connect(taxonomy, SIGNAL(selected()), this, SLOT(taxonomySelected()));

    grid->setRowStretch(0, 1);
    setLayout(grid);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_browser);
    pageBrowser->setLayout(vbox);
}

void RssViewWidget::loadRss(NvRssItem *rss)
{
    m_current = rss;

    taxonomy->clearSelection();
    if(!rss->terms().isEmpty()) {
        taxonomy->selectTaxonomy(rss->terms());
    }

    titleLabel->setText(rss->name());
    textLabel->setText(rss->description());

    QUrl url = rss->link();
    if(url.isValid()) {
        m_browser->setUrl(url);
    }

    /*
    QUrl url(rss->getLink());
    if(url.isValid()) {
        m_browser->setUrl(url);
    }
    */
}

void RssViewWidget::updateTaxonomy()
{
    ResourceManager *rm = ResourceManager::instance();
    QTreeWidgetItem* tax = rm->getTaxonomy();
    if(tax->childCount() > 0) {
        taxonomy->loadTaxonomy(tax);
    }
}

bool RssViewWidget::storeRss(NvRssItem *rss)
{
    bool updated = taxonomy->selectedTaxonomy() != rss->terms();


    if(updated) {
        rss->setTerms( taxonomy->selectedTaxonomy() );
        rss->setUpdated( true );
    }

    return true;
}

void RssViewWidget::showAttachLink(bool show)
{
    if(show)
        attachLink->show();
    else
        attachLink->hide();
}

void RssViewWidget::taxonomySelected()
{
    Q_ASSERT( m_current );
    storeRss( m_current );
}
