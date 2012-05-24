#include "mainwindow.h"
#include "connector.h"
#include "node.h"
#include "taxonomyterm.h"
#include "resourcemanager.h"
#include "centerlawidget.h"
#include "windowmanager.h"
#include "dbmanager.h"

#include "model/nvrssitem.h"
#include "model/Tag.h"
#include "model/NvMediaModel.h"
#include "view/NvBaseListView.h"
#include "view/NvFeedsTreeView.h"
#include "view/FilterWidget.h"
#include "view/FilterNodeView.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUI();
    setupActions();
    initToolbar();

    initWidgets();
    initStatusBar();
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
    messageLabel->hide();

    left = new QSplitter(Qt::Horizontal, this);


    // prepare filter model
    //m_rssFilterModel = new NvSortFilterModel(this);
    //m_rssFilterModel->setSourceModel( RM->rssModel() );
    //m_rssFilterModel->setDynamicSortFilter(true);

    rssList = new NvBaseListView(left);
    rssList->setViewMode(NvBaseListView::VIEW_LINE);
    rssList->setModelColumn(DBManager::RSS_FIELD_TITLE);
    rssList->setModel( RM->rssModel() );
    rssList->setEnabled( false );

    RM->rssModel()->select();

    //left->addWidget(feedsTree);
    left->addWidget(new FilterWidget(rssList, RM->feedModel(), left));
    leftSideBtn = new QPushButton(tr("<"), this);
    rightSideBtn = new QPushButton(tr(">"), this);
    leftSideBtn->setMaximumWidth(20);
    rightSideBtn->setMaximumWidth(20);
    leftSideBtn->hide();
    rightSideBtn->hide();

    connect(leftSideBtn, SIGNAL(clicked()), this, SLOT(collapseLeftPane()));
    connect(rightSideBtn, SIGNAL(clicked()), this, SLOT(collapseRightPane()));



    QWidget *middle = new QWidget(split);
    QVBoxLayout* cv = new QVBoxLayout;
    cv->setMargin(0);
    view = new CenterlaWidget(middle);
    cv->addWidget(messageLabel);
    cv->addWidget(view);
    middle->setLayout(cv);
    connect(rssList, SIGNAL(attachSelected(quint32)), view, SLOT(attachRss(quint32)));

    right = new QWidget(split);
    QGridLayout *gridRught = new QGridLayout();
    gridRught->setMargin(0);

    themesList = new QListView(right);
    themesList->setModel(RM->nodesModel());
    themesList->setEnabled(false);
    themesList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    gridRught->addWidget(new FilterNodeWiew(themesList, this), 0, 0, 1, 2);
    right->setLayout(gridRught);
    split->addWidget(left);
    //split->addWidget(leftSideBtn);
    split->addWidget(middle);
    //split->addWidget(rightSideBtn);
    split->addWidget(right);


    setCentralWidget(split);
}

void MainWindow::initToolbar()
{
    QToolBar *mainToolbar = addToolBar("main-toolbar");
    mainToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mainToolbar->setMovable(false);
    mainToolbar->addAction(syncAction);
    mainToolbar->addAction(galleryAction);
    mainToolbar->addSeparator();
    mainToolbar->addAction(themeAction);
}

void MainWindow::setupActions()
{
    QAction *a;

    syncAction = new QAction(QIcon(":/images/synchronize.png"), tr("Sync"), this);
    galleryAction = new QAction(QIcon(":/images/gallery.png"), tr("Gallery"), this);
    themeAction = new QAction(QIcon(":/images/new.png"), tr("New theme"), this);
    connect(themeAction, SIGNAL(triggered()), this, SLOT(createNode()));
    connect(syncAction, SIGNAL(triggered()), this, SLOT(syncClicked()));

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

    a = new QAction(tr("Change Layout"), this);
    connect(a, SIGNAL(triggered()), view, SLOT(changeViewMode()));
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
    view->showNode(Node(0, ""));
}

void MainWindow::initWidgets()
{
    ResourceManager *rm = ResourceManager::instance();
    Connector *c = WindowManager::instance()->connector();

    TagsManager::init();

    connect(rssList, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
    connect(rssList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemDoubleClicked(QModelIndex)));


    //rssList->setModel(&rm->getFeed());

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

void MainWindow::initStatusBar()
{
    statusBar()->setSizeGripEnabled(false);
    //statusBar()->addWidget(new QProgressBar(this));
    statusBar()->addWidget(new QLabel(this));
}

void MainWindow::userLoged()
{

}


void MainWindow::syncClicked()
{
    view->showDummy();
    Connector *c = WindowManager::instance()->connector();

    ResourceManager *rm = ResourceManager::instance();
    rssList->setEnabled(false);
    c->SyncRss();

    themesList->setEnabled(false);
    c->SyncNodes();

    MediaManager::sync();
}

void MainWindow::itemClicked(QModelIndex index)
{
    NvRssItem item;
    ResourceManager* rm = ResourceManager::instance();

    if(view->currentView() == CenterlaWidget::WIDGET_NODE)
        return;


    item = rm->rssModel()->item(index);

    if(item.id() != 0) {
        view->showRss(item);
    }
}

void MainWindow::itemDoubleClicked(QModelIndex index)
{
    NvRssItem* item;
    ResourceManager* rm = ResourceManager::instance();

    /*
    item = dynamic_cast<NvRssItem*>(rm->rssModel()->item(index));

    if(item) {
        view->showRss(item);
    }
    */
}

void MainWindow::loadNode(QModelIndex index)
{
    Connector *c = WindowManager::instance()->connector();
    NvNodeModel *m = ResourceManager::instance()->nodesModel();

    Node item = m->item(index);
    if(item.id() > 0) {
        if(item.getBody().isEmpty()) {
            c->GetNode(item.id());
        }else{
           view->showNode(item);
        }
    }
}


void MainWindow::nodesLoaded()
{
    themesList->setEnabled(true);
}

void MainWindow::rssLoaded()
{
    rssList->setEnabled(true);
    //m_rssFilterModel->invalidate();
}

void MainWindow::nodeLoaded(Node *node)
{
    if(node) {
        view->showNode(*node);
    }
}


void MainWindow::networkError(QString msg)
{
    showError(msg);

    view->showLogin();
    rssList->setEnabled(false);
    themesList->setEnabled(false);
}

void MainWindow::actionLogin(QString userLogin, QString userPassword)
{
    Connector *c = WindowManager::instance()->connector();
    c->Login(userLogin, userPassword);
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

void MainWindow::collapseLeftPane()
{
    if(left->isVisible()) {
        left->hide();
        leftSideBtn->setText(tr(">"));
    }else{
        leftSideBtn->setText(tr("<"));
        left->show();
    }
}

void MainWindow::collapseRightPane()
{
    if(right->isVisible()) {
        right->hide();
        rightSideBtn->setText(tr("<"));
    }else{
        rightSideBtn->setText(tr(">"));
        right->show();
    }
}
