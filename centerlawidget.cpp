#include "centerlawidget.h"
#include "rssviewwidget.h"
#include "nodeeditorwidget.h"
#include "node.h"
#include "model/nvrssitem.h"
#include <QtGui>
#include <QSplitter>

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
    QFrame(parent), m_currentRss(NULL), _prevIndex(0)
{
    setObjectName("center");

    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    stacked = new QStackedWidget(this);

    m_btnNav = new QPushButton(tr("Go"), this);
    m_btnAttach = new QPushButton(tr("Attach"), this);
    m_titleLabel = new QLabel(this);

    hbox->addWidget(m_titleLabel, 1);
    hbox->addWidget(m_btnNav);
    hbox->addWidget(m_btnAttach);
    hbox->setMargin(0);

    vbox->addWidget(stacked);
    vbox->addLayout(hbox);
    vbox->setMargin(0);

    m_rssView = new RssViewWidget(stacked);
    m_nodeView = new NodeEditorWidget(stacked);
    m_dummyView = new QWidget(stacked);
    m_loginView = new LoginWidget(stacked);
    splitWidget = new QSplitter(stacked);

    splitWidget->setOrientation(Qt::Horizontal);

    stacked->addWidget(m_dummyView);
    stacked->addWidget(m_loginView);
    stacked->addWidget(splitWidget);


    setViewMode(VIEW_SPLIT);

    connect(m_btnNav, SIGNAL(clicked()), this, SLOT(navigateToOther()));
    connect(m_btnAttach, SIGNAL(clicked()), this, SLOT(attachRss()));
    connect(m_rssView, SIGNAL(attachClicked()), this, SLOT(attachRss()));

    m_btnNav->hide();


    setLayout(vbox);
}

void CenterlaWidget::showNode(const Node &node)
{
    if(!m_nodeView->loadNode(node)) {
        QMessageBox::critical(this, tr("Error"), tr("Can load node"));
    }

    showView(WIDGET_NODE);
    showLinkTo(2);
}

int CenterlaWidget::currentView() const
{
    return stacked->currentIndex();
}

void CenterlaWidget::showRss(const NvRssItem &rss)
{
    if(m_currentRss.id()) {
       m_rssView->storeRss(&m_currentRss);
    }

    m_currentRss = rss;
    m_rssView->loadRss(&m_currentRss);
    m_rssView->showAttachLink( true  );

   showView(WIDGET_RSS);
   showLinkTo(1);
}

void CenterlaWidget::showDummy()
{
    m_currentRss.setId(0);
    m_nodeView->clear();
    showLinkTo();
    if(stacked->currentIndex() != WIDGET_DUMMY) {
        stacked->setCurrentIndex(WIDGET_DUMMY);
    }
}

void CenterlaWidget::showLogin()
{
    m_currentRss.setId(0);
    m_nodeView->clear();
    showLinkTo();
    if(stacked->currentIndex() != WIDGET_LOGIN) {
        stacked->setCurrentIndex(WIDGET_LOGIN);
    }
}

void CenterlaWidget::updateTaxonomy()
{

}

void CenterlaWidget::nodeAttachRss(quint32 rss_id)
{
    m_nodeView->attachRss(rss_id);
}

void CenterlaWidget::setLogin(const QString &login, const QString &password)
{
    emit actionLogin(login, password);
}

void CenterlaWidget::showLinkTo(int type)
{
    QString title;
    QString str;

    m_btnNav->hide();
    m_titleLabel->hide();
    m_btnAttach->hide();

    if(_viewMode == VIEW_SPLIT)
        return;

    if(type == 2) // current view node
    {
        m_titleLabel->setText(QString("Current news: ") + m_currentRss.name());
        m_btnNav->show();
        m_titleLabel->show();
        m_btnAttach->show();
    }else if(type == 1) {
           title = m_nodeView->currentNode()->title();
           if(m_nodeView->currentNode()->status() == Node::STATUS_COMPOSED)
                    title = tr("Composed theme");

           m_titleLabel->setText(QString("Current theme: %1").arg(title));
           m_btnNav->show();
           m_titleLabel->show();
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
    Q_ASSERT(m_currentRss.id());

    nodeAttachRss(m_currentRss.id());
}

void CenterlaWidget::attachRss(quint32 rss_id)
{
    nodeAttachRss(rss_id);
}

void CenterlaWidget::setViewMode(int mode)
{
    stacked->removeWidget(m_rssView);
    stacked->removeWidget(m_nodeView);
    m_rssView->setVisible(false);
    m_nodeView->setVisible(false);

    int index = stacked->currentIndex();

    if(mode == VIEW_SPLIT) {
        splitWidget->addWidget(m_rssView);
        splitWidget->addWidget(m_nodeView);

        m_rssView->setVisible(true);
        m_nodeView->setVisible(true);
    }else{
        stacked->addWidget(m_rssView);
        stacked->addWidget(m_nodeView);

        if(_prevIndex == WIDGET_RSS)
             m_rssView->setVisible(true);
        if(_prevIndex == WIDGET_NODE)
            m_nodeView->setVisible(true);
        showView(_prevIndex);
    }


    m_rssView->update();
    m_rssView->updateGeometry();
    m_rssView->repaint();
    m_rssView->adjustSize();

    m_nodeView->update();
    m_nodeView->repaint();
    m_nodeView->adjustSize();
    splitWidget->adjustSize();

    if(mode == VIEW_SPLIT) {
            if(index != WIDGET_RSS && index != WIDGET_NODE)
               _prevIndex = WIDGET_RSS;
            else
               _prevIndex = index;
    }
    _viewMode = mode;


}

void CenterlaWidget::showView(int view)
{
    if(_viewMode == VIEW_STACKED) {
        if(stacked->currentIndex() != view) {
            stacked->setCurrentIndex(view);
        }

    }else{
       stacked->setCurrentIndex(WIDGET_SPLIT);
    }
}

void CenterlaWidget::changeViewMode()
{
    int mode = _viewMode == VIEW_STACKED ? VIEW_SPLIT : VIEW_STACKED;
    setViewMode( mode );
}
