#include "centerlawidget.h"
#include "rssviewwidget.h"
#include "nodeeditorwidget.h"
#include "node.h"
#include "rssitem.h"
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

    CenterlaWidget * cnt = static_cast<CenterlaWidget*>(this->parent());
    if(cnt) {
        cnt->setLogin(userLogin, userPassword);
    }
}


CenterlaWidget::CenterlaWidget(QWidget *parent) :
    QStackedWidget(parent), m_currentRss(NULL)
{
    m_mainWidget = new QWidget(this);
    m_rssView = new RssViewWidget(m_mainWidget);
    m_nodeView = new NodeEditorWidget(m_mainWidget);
    m_dummyView = new QWidget(this);
    m_loginView = new LoginWidget(this);

    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(m_rssView);
    lay->addWidget(m_nodeView);
    m_mainWidget->setLayout(lay);

    addWidget(m_mainWidget);
    addWidget(m_dummyView);
    addWidget(m_loginView);
}

void CenterlaWidget::showNode(Node *node)
{
    m_nodeView->loadNode(node);

    if(currentIndex() != WIDGET_MAIN) {
        setCurrentIndex(WIDGET_MAIN);
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

    if(currentIndex() != WIDGET_MAIN) {
        setCurrentIndex(WIDGET_MAIN);
    }
}

void CenterlaWidget::showDummy()
{
    m_currentRss = NULL;
    m_nodeView->clear();
    if(currentIndex() != WIDGET_DUMMY) {
        setCurrentIndex(WIDGET_DUMMY);
    }
}

void CenterlaWidget::showLogin()
{
    m_currentRss = NULL;
    m_nodeView->clear();
    if(currentIndex() != WIDGET_LOGIN) {
        setCurrentIndex(WIDGET_LOGIN);
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

void CenterlaWidget::setLogin(const QString &login, const QString &password)
{
    emit actionLogin(login, password);
}
