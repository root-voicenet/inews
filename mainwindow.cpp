#include "mainwindow.h"
#include "connector.h"
#include "node.h"
#include "rssitem.h"
#include "taxonomyterm.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include "centerlawidget.h"
#include "rsslistitemdelegate.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUI();
    setupDockablePanels();
    initWidgets();
}

MainWindow::~MainWindow()
{
    if(m_connector)
        delete m_connector;
}

void MainWindow::setupUI()
{
    resize(800, 612);

    QWidget *centralwidget = new QWidget(this);
    messageLabel = new QLabel(centralwidget);
    QHBoxLayout*  hbox = new QHBoxLayout(centralwidget);
    QVBoxLayout* cv = new QVBoxLayout;

    view = new CenterlaWidget(centralwidget);
    cv->addWidget(messageLabel);
    cv->addWidget(view);
    hbox->addLayout(cv);

    QGridLayout *gridRught = new QGridLayout();
    gridRught->addWidget(new QLabel(centralwidget), 0, 0, 1, 1);

    themesList = new QListView(centralwidget);
    themesList->setEnabled(false);
    gridRught->addWidget(themesList, 1, 0, 1, 2);
    btnNew = new QPushButton(tr("New Theme"), this);
    btnNew->setEnabled(false);
    gridRught->addWidget(btnNew, 2, 0, 1, 2);
    connect(btnNew, SIGNAL(clicked()), this, SLOT(createNode()));

    gridRught->addWidget(new QPushButton(centralwidget), 5, 1, 1, 1);

    btnSync = new QPushButton(tr("Sync"), centralwidget);
    btnSync->setMaximumSize(QSize(16777215, 50));
    gridRught->addWidget(btnSync, 6, 0, 1, 2);
    gridRught->addWidget(new QLabel(centralwidget), 3, 0, 1, 1);


    hbox->addLayout(gridRught);

    hbox->setStretch(1, 0);
    setCentralWidget(centralwidget);
}

void MainWindow::showError(const QString& str)
{
    messageLabel->setText(str);
    messageLabel->setMinimumHeight(50);
    messageLabel->setStyleSheet("QLabel { background-color : pink; border-radius: 10px; border-color: red; padding: 6px;}");

}

void MainWindow::createNode()
{
    view->showNode(NULL);
}

void MainWindow::setupDockablePanels()
{
    dock = new QDockWidget(tr("Recent News"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    QWidget *central = new QWidget(dock);

    QBoxLayout *box = new QBoxLayout(QBoxLayout::TopToBottom);
    rssList = new QListView(central);
    rssList->setItemDelegate(new RssListItemDelegate());

    box->addWidget(rssList);
    central->setLayout(box);
    dock->setWidget(central);
    connect(dock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(dockLocationChanged(Qt::DockWidgetArea)));

    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::initWidgets()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();

    connect(rssList, SIGNAL(clicked(QModelIndex)), this, SLOT(rssItemSelected(QModelIndex)));

    themesList->setModel(&rm->getThemes());
    rssList->setModel(&rm->getFeed());
    connect(rssList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(attachRss(QModelIndex)));

    m_connector = new Connector("http://test.irkipedia.ru/news/api");

    connect(btnSync, SIGNAL(clicked()), this, SLOT(syncClicked()));
    connect(themesList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadNode(QModelIndex)));
    connect(view, SIGNAL(actionLogin(QString, QString)), this, SLOT(actionLogin(QString, QString)));
    connect(m_connector, SIGNAL(taxonomyLoaded()), view, SLOT(updateTaxonomy()));
    connect(m_connector, SIGNAL(syncNodesComplete()), this, SLOT(nodesLoaded()));
    connect(m_connector, SIGNAL(syncRssComplete()), this, SLOT(rssLoaded()));
    connect(m_connector, SIGNAL(nodeGetComplete(Node*)), this, SLOT(nodeLoaded(Node*)));
    connect(m_connector, SIGNAL(networkError(QString)), this, SLOT(networkError(QString)));

    view->showLogin();
}


void MainWindow::syncClicked()
{
    view->showDummy();

    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    rssList->setEnabled(false);
    m_connector->SyncRss(rm->getUpdatedRss());

    themesList->setEnabled(false);
    btnNew->setEnabled(false);
    m_connector->SyncNodes(rm->getUpdatedNodes());
}

void MainWindow::rssItemSelected(QModelIndex index)
{
    RssItem *rss;

    QStandardItemModel *model = static_cast<QStandardItemModel*>(rssList->model());
    QStandardItem *item = model->itemFromIndex(index);

    if(item) {
        // apply rss tids
        rss = reinterpret_cast<RssItem*>(item->data().toInt());
        view->showRss(rss);
    }
}


void MainWindow::dockLocationChanged(Qt::DockWidgetArea area)
{
    QWidget *central = dock->widget();
    QBoxLayout *box = static_cast<QBoxLayout*>(central->layout());
    if(area == Qt::TopDockWidgetArea) {
        box->setDirection(QBoxLayout::LeftToRight);
    }else{
        box->setDirection(QBoxLayout::TopToBottom);
    }

    central->adjustSize();
    adjustSize();
}

void MainWindow::loadNode(QModelIndex index)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(themesList->model());
    QStandardItem *item = model->itemFromIndex(index);
    if(item && !item->data().isNull()) {
        Node* node = reinterpret_cast<Node*>(item->data().toInt());
        if(node->isRemote() && node->getBody().isEmpty()) {
            m_connector->GetNode(node->getId());
        }else{
           view->showNode(node);
        }
    }
}


void MainWindow::nodesLoaded()
{
    themesList->setEnabled(true);
    btnNew->setEnabled(true);
}

void MainWindow::rssLoaded()
{
    rssList->setEnabled(true);
}

void MainWindow::nodeLoaded(Node *node)
{
    if(node) {
        view->showNode(node);
    }
}


void MainWindow::attachRss(QModelIndex index)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(rssList->model());
    QStandardItem *item = model->itemFromIndex(index);

    if(item && !item->data().isNull()) {
        RssItem* rss = reinterpret_cast<RssItem*>(item->data().toInt());
        if(rss) {
            view->nodeAttachRss(rss);
        }
    }
}

void MainWindow::networkError(QString msg)
{
    showError(msg);


    view->showLogin();
    rssList->setEnabled(false);
    themesList->setEnabled(false);
    btnNew->setEnabled(false);
    btnSync->setEnabled(false);
}

void MainWindow::actionLogin(QString userLogin, QString userPassword)
{
    m_connector->Login(userLogin, userPassword);
    btnSync->setEnabled(true);
    view->showDummy();
}

