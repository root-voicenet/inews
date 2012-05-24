#include "rssviewwidget.h"
#include "model/nvrssitem.h"
#include "resourcemanager.h"
#include "view/TagsWidget.h"
#include <QtGui>
#include <QWebView>

RssViewWidget::RssViewWidget(QWidget *parent) :
    QFrame(parent), m_current(NULL)
{
    setObjectName("rss-view");
    setupUI();
}

void RssViewWidget::setupUI()
{
    QLabel *headingLabel = new QLabel(tr("Current message"), this);
    headingLabel->setObjectName("heading");

    m_browser = new QWebView(this);
    m_browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_browser->hide();

    QToolButton *btBrowser = new QToolButton(this);
    btBrowser->setIcon(QIcon(":/images/world.png"));
    connect(btBrowser, SIGNAL(clicked()), this, SLOT(showHideBrowser()));


    dateLabel = new QLabel(this);
    dateLabel->setObjectName("date");

    sourceLabel = new QLabel(this);
    sourceLabel->setObjectName("source");

    // set overwiev page layout
    titleLabel = new QLabel(this);
    titleLabel->setObjectName("title");
    titleLabel->setWordWrap(true);

    textLabel = new QLabel(this);
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    textLabel->setWordWrap(true);
    textLabel->setObjectName("desc");
    tagsWidget = new TagsWidget(this);

    QToolButton *btShowBrowser = new QToolButton;
    btShowBrowser->setText("b");
    btShowBrowser->hide();
    connect(btShowBrowser, SIGNAL(clicked()), this, SLOT(showHideBrowser()));

    vbox = new QVBoxLayout;
    vbox->addWidget(headingLabel);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(dateLabel, 0, Qt::AlignLeft);
    QHBoxLayout *sbox = new QHBoxLayout;
    sbox->setMargin(0);
    sbox->addWidget(btBrowser);
    sbox->addWidget(sourceLabel);
    hbox->addLayout(sbox, 0);
    hbox->addStretch(1);
    vbox->addLayout(hbox);
    vbox->addWidget(titleLabel, 0, Qt::AlignTop);
    vbox->addWidget(m_browser, 0, Qt::AlignTop);
    vbox->addWidget(textLabel, 0, Qt::AlignTop);
    vbox->addWidget(tagsWidget);
    vbox->addStretch(1);

    connect(tagsWidget, SIGNAL(changed()), this, SLOT(tagsChanged()));
    setLayout(vbox);
}

void RssViewWidget::loadRss(NvRssItem *rss)
{
    m_current = rss;

    tagsWidget->clear();
    tagsWidget->loadTags(rss->tags());

    titleLabel->setText(rss->name());
    textLabel->setText(rss->description());
    dateLabel->setText(rss->date().toString(Qt::SystemLocaleDate));
    sourceLabel->setText(rss->link());

    QUrl url = rss->link();
    if(url.isValid() && m_browser->isVisible()) {
        m_browser->setUrl(url);
    }
}

bool RssViewWidget::storeRss(NvRssItem *rss)
{
   bool updated = tagsWidget->selectedTags() != rss->tags();
   rss->setUpdated( updated );
   rss->setReaded( true );

   if(updated) {
        rss->setTags( tagsWidget->selectedTags() );
   }

   NvRssCachedModel *model =  ResourceManager::instance()->rssModel();

   model->save( *rss );

    return true;
}

void RssViewWidget::showAttachLink(bool show)
{
    /*
    if(show)
        attachLink->show();
    else
        attachLink->hide();
    */
}

void RssViewWidget::tagsChanged()
{
    Q_ASSERT( m_current );
    storeRss( m_current );
}


void RssViewWidget::showHideBrowser()
{
    m_browser->setHidden( m_browser->isVisible() );
    textLabel->setHidden( textLabel->isVisible() );

    if(!m_current)
        return;

    QUrl url = m_current->link();
    if(m_browser->url() != url) {
        m_browser->setUrl( url );
    }
}
