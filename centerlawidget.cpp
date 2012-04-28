#include "centerlawidget.h"
#include "rssviewwidget.h"
#include "nodeeditorwidget.h"
#include "node.h"
#include "model/nvrssitem.h"
#include <QtGui>

LoginWidget::LoginWidget(QWidget *parent)
     : QWidget(parent)
{

        userEdit = new QLineEdit(this);
        passwordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);


        QGridLayout* grid = new QGridLayout;
        grid->addWidget(new QLabel("Wellcome to iNews", this), 0, 0, 1, 2);
        grid->addWidget(new QLabel(tr("User name"), this), 1, 0, 1, 1);
        grid->addWidget(userEdit, 1, 1, 1, 1);

        grid->addWidget(new QLabel(tr("User password"), this), 2, 0, 1, 1);
        grid->addWidget(passwordEdit, 2, 1, 1, 1);

        QPushButton *btnSubmit = new QPushButton(tr("Login"), this);
        connect(btnSubmit, SIGNAL(clicked()), this, SLOT(submitClicked()));
        grid->addWidget(btnSubmit, 3, 0, 1, 2);

        setMaximumHeight(200);
        setLayout(grid);
}



void LoginWidget::submitClicked()
{
    QString userLogin = userEdit->text();
    QString userPassword = passwordEdit->text();

    CenterlaWidget * cnt = static_cast<CenterlaWidget*>(this->parent()->parent());
    if(cnt) {
        cnt->setLogin(userLogin, userPassword);
    }
}


CenterlaWidget::CenterlaWidget(QWidget *parent) :
    QWidget(parent), m_currentRss(NULL), m_currentNode(NULL)
{
    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    stacked = new QStackedWidget(this);
    m_btnNav = new QPushButton(tr("Go"), this);
    m_btnAttach = new QPushButton(tr("Attach"), this);
    m_titleLabel = new QLabel(this);

    hbox->addWidget(m_titleLabel, 1);
    hbox->addWidget(m_btnNav);
    hbox->addWidget(m_btnAttach);
    vbox->addWidget(stacked, 1);
    vbox->addLayout(hbox);
    vbox->setMargin(0);

    m_rssView = new RssViewWidget(stacked);
    m_nodeView = new NodeEditorWidget(stacked);
    m_dummyView = new QWidget(stacked);
    m_loginView = new LoginWidget(stacked);

    stacked->addWidget(m_rssView);
    stacked->addWidget(m_nodeView);
    stacked->addWidget(m_dummyView);
    stacked->addWidget(m_loginView);

    connect(m_btnNav, SIGNAL(clicked()), this, SLOT(navigateToOther()));
    connect(m_btnAttach, SIGNAL(clicked()), this, SLOT(attachRss()));
    connect(m_rssView, SIGNAL(attachClicked()), this, SLOT(attachRss()));

    m_btnNav->hide();


    setLayout(vbox);
}

void CenterlaWidget::showNode(Node *node)
{
    m_currentNode = m_nodeView->loadNode(node);
    Q_ASSERT(m_currentNode);

    if(stacked->currentIndex() != WIDGET_NODE) {
        stacked->setCurrentIndex(WIDGET_NODE);
    }

    showLinkTo(2);
}

int CenterlaWidget::currentView() const
{
    return stacked->currentIndex();
}

void CenterlaWidget::showRss(NvRssItem *rss)
{
    if(m_currentRss) {
        if(!m_rssView->storeRss(m_currentRss))
            return;
    }

    m_rssView->loadRss(rss);
    m_rssView->showAttachLink( m_currentNode != NULL );
    m_currentRss = rss;

    if(stacked->currentIndex() != WIDGET_RSS) {
        stacked->setCurrentIndex(WIDGET_RSS);
    }
    showLinkTo(1);
}

void CenterlaWidget::showDummy()
{
    m_currentRss = NULL;
    m_currentNode = NULL;
    m_nodeView->clear();
    showLinkTo();
    if(stacked->currentIndex() != WIDGET_DUMMY) {
        stacked->setCurrentIndex(WIDGET_DUMMY);
    }
}

void CenterlaWidget::showLogin()
{
    m_currentRss = NULL;
    m_currentNode = NULL;
    m_nodeView->clear();
    showLinkTo();
    if(stacked->currentIndex() != WIDGET_LOGIN) {
        stacked->setCurrentIndex(WIDGET_LOGIN);
    }
}

void CenterlaWidget::updateTaxonomy()
{
    m_rssView->updateTaxonomy();
    m_nodeView->updateTaxonomy();
}

void CenterlaWidget::nodeAttachRss(NvRssItem *rss)
{
    m_nodeView->attachRss(rss);
}

void CenterlaWidget::setLogin(const QString &login, const QString &password)
{
    emit actionLogin(login, password);
}

void CenterlaWidget::showLinkTo(int type)
{
    QString title;
    QString str;
    if(type == 2 && m_currentRss) // current view node
    {
        m_titleLabel->setText(QString("Current news: ") + m_currentRss->name());
        m_btnNav->show();
        m_titleLabel->show();
        m_btnAttach->show();
    }else{
        if(type == 1 && m_currentNode) {
           title = m_currentNode->getTitle();
           if(title.isEmpty())
                    title = tr("Edited Theme");

           m_titleLabel->setText(QString("Current News: ") + title);
           m_btnNav->show();
           m_titleLabel->show();
        }else{
            m_btnNav->hide();
            m_titleLabel->hide();
        }
        m_btnAttach->hide();
    }
}

void CenterlaWidget::navigateToOther()
{
    if(stacked->currentIndex() == WIDGET_RSS) {
        stacked->setCurrentIndex(WIDGET_NODE);
        showLinkTo(2);
    }else{
        stacked->setCurrentIndex(WIDGET_RSS);
        showLinkTo(1);
    }
}

void CenterlaWidget::attachRss()
{
    Q_ASSERT(m_currentRss);

    nodeAttachRss(m_currentRss);
}

void CenterlaWidget::attachRss(NvRssItem *item)
{
    nodeAttachRss(item);
}
