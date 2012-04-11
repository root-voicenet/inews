#include "centerlawidget.h"
#include "rssviewwidget.h"
#include "nodeeditorwidget.h"
#include "node.h"
#include "rssitem.h"
#include <QStackedWidget>
#include <QBoxLayout>


CenterlaWidget::CenterlaWidget(QWidget *parent) :
    QWidget(parent), m_currentRss(NULL)
{
    m_layout = new QStackedWidget(this);
    m_rssView = new RssViewWidget(this);
    m_nodeView = new NodeEditorWidget(this);
    m_dummyView = new QWidget(this);

    m_layout->addWidget(m_rssView);
    m_layout->addWidget(m_nodeView);
    m_layout->addWidget(m_dummyView);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(m_layout);
    setLayout(vbox);
}

void CenterlaWidget::showNode(Node *node)
{
    m_nodeView->loadNode(node);

    if(m_layout->currentIndex() != WIDGET_NODE) {
        m_layout->setCurrentIndex(WIDGET_NODE);
    }
}

void CenterlaWidget::showRss(RssItem *rss)
{
    if(m_currentRss) {
        if(!m_rssView->storeRss(m_currentRss))
            return;
    }

    m_rssView->loadRss(rss);
    m_currentRss = rss;

    if(m_layout->currentIndex() != WIDGET_RSS) {
        m_layout->setCurrentIndex(WIDGET_RSS);
    }
}

void CenterlaWidget::showDummy()
{
    m_currentRss = NULL;
    m_nodeView->clear();
    if(m_layout->currentIndex() != WIDGET_DUMMY) {
        m_layout->setCurrentIndex(WIDGET_DUMMY);
    }
}


void CenterlaWidget::updateTaxonomy()
{
    m_rssView->updateTaxonomy();
}

void CenterlaWidget::nodeAttachRss(RssItem *rss)
{
    m_nodeView->attachRss(rss);
}
