#include "mainwindow.h"
#include "connector.h"
#include "node.h"
#include "taxonomyterm.h"
#include "resourcemanager.h"
#include "centerlawidget.h"

#include "model/nvrssitem.h"
#include "view/NvBaseListView.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUI();
    setupActions();
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

    QSplitter*  split = new QSplitter(Qt::Horizontal, this);
    messageLabel = new QLabel(split);



    QSplitter* left = new QSplitter(Qt::Horizontal, this);
    feedsTree = new QTreeWidget(left);
    rssList = new NvBaseListView(left);
    rssList->setViewMode(NvBaseListView::VIEW_LINE);
    rssList->setModel( ResourceManager::instance()->rssModel() );
    left->addWidget(feedsTree);
    left->addWidget(rssList);


    QWidget *middle = new QWidget(split);
    QVBoxLayout* cv = new QVBoxLayout;
    view = new CenterlaWidget(middle);
    cv->addWidget(messageLabel);
    cv->addWidget(view);
    middle->setLayout(cv);


    QWidget *right = new QWidget(split);
    QGridLayout *gridRught = new QGridLayout();
    gridRught->addWidget(new QLabel(right), 0, 0, 1, 1);
    themesList = new QListView(right);
    themesList->setEnabled(false);
    gridRught->addWidget(themesList, 1, 0, 1, 2);
    btnNew = new QPushButton(tr("New Theme"), this);
    btnNew->setEnabled(false);
    gridRught->addWidget(btnNew, 2, 0, 1, 2);
    connect(btnNew, SIGNAL(clicked()), this, SLOT(createNode()));

    gridRught->addWidget(new QPushButton(right), 5, 1, 1, 1);

    btnSync = new QPushButton(tr("Sync"), right);
    btnSync->setMaximumSize(QSize(16777215, 50));
    gridRught->addWidget(btnSync, 6, 0, 1, 2);
    gridRught->addWidget(new QLabel(right), 3, 0, 1, 1);
    right->setLayout(gridRught);

    split->addWidget(left);
    split->addWidget(middle);
    split->addWidget(right);


    setCentralWidget(split);
}

void MainWindow::setupActions()
{
    QAction *a;

    // set file menu actions
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    a = new QAction(tr("&Quit"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(a);

    QMenu* viewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(viewMenu);
    a = new QAction(tr("RSS List view"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(setListViewMode()));
    viewMenu->addAction(a);

    a = new QAction(tr("RSS Full view"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(setFullViewMode()));
    viewMenu->addAction(a);
}

void MainWindow::showError(const QString& str)
{
    messageLabel->setText(str);
    messageLabel->setMinimumHeight(50);
    //messageLabel->setStyleSheet("QLabel { background-color : pink; border-radius: 10px; border-color: red; padding: 6px;}");

}

void MainWindow::createNode()
{
    view->showNode(NULL);
}

void MainWindow::initWidgets()
{
    ResourceManager *rm = ResourceManager::instance();

    connect(rssList, SIGNAL(activated(QModelIndex)), this, SLOT(rssItemSelected(QModelIndex)));

    themesList->setModel(&rm->getThemes());
    //rssList->setModel(&rm->getFeed());
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

    ResourceManager *rm = ResourceManager::instance();
    rssList->setEnabled(false);
    m_connector->SyncRss();

    themesList->setEnabled(false);
    btnNew->setEnabled(false);
    m_connector->SyncNodes(rm->getUpdatedNodes());
}

void MainWindow::rssItemSelected(QModelIndex index)
{
    NvRssItem* item;
    ResourceManager* rm = ResourceManager::instance();

    item = dynamic_cast<NvRssItem*>(rm->rssModel()->item(index));
    if(item) {
        view->showRss(item);
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
    /*
    QStandardItemModel *model = static_cast<QStandardItemModel*>(rssList->model());
    QStandardItem *item = model->itemFromIndex(index);

    if(item && !item->data().isNull()) {
        RssItem* rss = reinterpret_cast<RssItem*>(item->data().toInt());
        if(rss) {
            view->nodeAttachRss(rss);
        }
    }
    */
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

void MainWindow::setListViewMode()
{
    rssList->setViewMode(NvBaseListView::VIEW_LINE);
}

void MainWindow::setFullViewMode()
{
    rssList->setViewMode(NvBaseListView::VIEW_FULL);
}
