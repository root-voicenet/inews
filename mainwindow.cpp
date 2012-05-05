#include "mainwindow.h"
#include "connector.h"
#include "node.h"
#include "taxonomyterm.h"
#include "resourcemanager.h"
#include "centerlawidget.h"
#include "windowmanager.h"

#include "model/nvrssitem.h"
#include "model/NvSortFilterModel.h"
#include "view/NvBaseListView.h"
#include "view/NvFeedsTreeView.h"
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

}

void MainWindow::setupUI()
{
    resize(800, 612);
    ResourceManager *RM = ResourceManager::instance();

    QSplitter*  split = new QSplitter(Qt::Horizontal, this);
    messageLabel = new QLabel(split);



    QSplitter* left = new QSplitter(Qt::Horizontal, this);
    feedsTree = new NvFeedsTreeView(left);

    // prepare filter model
    m_rssFilterModel = new NvSortFilterModel(this);
    m_rssFilterModel->setSourceModel( RM->rssModel() );
    m_rssFilterModel->setDynamicSortFilter(true);

    rssList = new NvBaseListView(left);
    rssList->setViewMode(NvBaseListView::VIEW_LINE);
    rssList->setModel( m_rssFilterModel );
    connect(feedsTree, SIGNAL(feedClicked(int)), m_rssFilterModel, SLOT(setFeedId(int)));

    left->addWidget(feedsTree);
    left->addWidget(rssList);



    QWidget *middle = new QWidget(split);
    QVBoxLayout* cv = new QVBoxLayout;
    cv->setMargin(0);
    view = new CenterlaWidget(middle);
    cv->addWidget(messageLabel);
    cv->addWidget(view);
    middle->setLayout(cv);
    connect(rssList, SIGNAL(attachSelected(NvRssItem*)), view, SLOT(attachRss(NvRssItem*)));

    QWidget *right = new QWidget(split);
    QGridLayout *gridRught = new QGridLayout();
    gridRught->setMargin(0);
    themesList = new QListView(right);
    themesList->setEnabled(false);
    gridRught->addWidget(themesList, 0, 0, 1, 2);
    btnNew = new QPushButton(tr("New Theme"), this);
    btnNew->setEnabled(false);
    gridRught->addWidget(btnNew, 1, 0, 1, 2);
    connect(btnNew, SIGNAL(clicked()), this, SLOT(createNode()));

    gridRught->addWidget(new QPushButton(right), 5, 1, 1, 1);

    btnSync = new QPushButton(tr("Sync"), right);
    btnSync->setMaximumSize(QSize(16777215, 50));
    btnSync->setEnabled( false );
    gridRught->addWidget(btnSync, 6, 0, 1, 2);
    gridRught->addWidget(new QLabel(right), 2, 0, 1, 1);
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

    fileMenu->addAction(feedsTree->createFeedAction());

    QMenu* viewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(viewMenu);
    a = new QAction(tr("RSS List view"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(setListViewMode()));
    viewMenu->addAction(a);

    a = new QAction(tr("RSS Full view"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(setFullViewMode()));
    viewMenu->addAction(a);

    QMenu* windowMenu = new QMenu(tr("&Window"), this);
    menuBar()->addMenu(windowMenu);
    a = new QAction(tr("Show Media Browser"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(showMedia()));
    windowMenu->addAction(a);
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
    Connector *c = WindowManager::instance()->connector();

    connect(rssList, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
    connect(rssList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemDoubleClicked(QModelIndex)));

    themesList->setModel(&rm->getThemes());
    //rssList->setModel(&rm->getFeed());


    connect(btnSync, SIGNAL(clicked()), this, SLOT(syncClicked()));
    connect(themesList, SIGNAL(clicked(QModelIndex)), this, SLOT(loadNode(QModelIndex)));
    connect(view, SIGNAL(actionLogin(QString, QString)), this, SLOT(actionLogin(QString, QString)));
    connect(c, SIGNAL(taxonomyLoaded()), view, SLOT(updateTaxonomy()));
    connect(c, SIGNAL(syncNodesComplete()), this, SLOT(nodesLoaded()));
    connect(c, SIGNAL(syncRssComplete()), this, SLOT(rssLoaded()));
    connect(c, SIGNAL(nodeGetComplete(Node*)), this, SLOT(nodeLoaded(Node*)));
    connect(c, SIGNAL(networkError(QString)), this, SLOT(networkError(QString)));
    connect(c, SIGNAL(feedsLoaded()), rssList, SLOT(doItemsLayout()));
    connect(c, SIGNAL(logInFinished()), this, SLOT(userLoged()));

    view->showLogin();
}

void MainWindow::userLoged()
{
    btnSync->setEnabled( true );
}


void MainWindow::syncClicked()
{
    view->showDummy();
    Connector *c = WindowManager::instance()->connector();

    ResourceManager *rm = ResourceManager::instance();
    rssList->setEnabled(false);
    c->SyncRss();

    themesList->setEnabled(false);
    btnNew->setEnabled(false);
    c->SyncNodes();

    c->GetMedia();
}

void MainWindow::itemClicked(QModelIndex index)
{
    NvRssItem* item;
    ResourceManager* rm = ResourceManager::instance();

    if(view->currentView() == CenterlaWidget::WIDGET_NODE)
        return;

    item = dynamic_cast<NvRssItem*>(rm->rssModel()->item(index));

    if(item) {
        view->showRss(item);
    }
}

void MainWindow::itemDoubleClicked(QModelIndex index)
{
    NvRssItem* item;
    ResourceManager* rm = ResourceManager::instance();

    item = dynamic_cast<NvRssItem*>(rm->rssModel()->item(index));

    if(item) {
        view->showRss(item);
    }
}

void MainWindow::loadNode(QModelIndex index)
{
    Connector *c = WindowManager::instance()->connector();

    QStandardItemModel *model = static_cast<QStandardItemModel*>(themesList->model());
    QStandardItem *item = model->itemFromIndex(index);
    if(item && !item->data().isNull()) {
        Node* node = reinterpret_cast<Node*>(item->data().toInt());
        if(node->isRemote() && node->getBody().isEmpty()) {
            c->GetNode(node->getId());
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
    m_rssFilterModel->invalidate();
}

void MainWindow::nodeLoaded(Node *node)
{
    if(node) {
        view->showNode(node);
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
    Connector *c = WindowManager::instance()->connector();
    c->Login(userLogin, userPassword);
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

void MainWindow::showMedia()
{
    WindowManager::instance()->showMediaWindow();
}
