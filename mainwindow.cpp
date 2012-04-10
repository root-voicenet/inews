#include "mainwindow.h"
#include "connector.h"
#include "node.h"
#include "rssitem.h"
#include "taxonomyterm.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include "textedit.h"
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
    QHBoxLayout*  hbox = new QHBoxLayout(centralwidget);

    textEdit = new TextEdit(centralwidget);
    titleEdit = new QLineEdit(centralwidget);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(new QLabel(tr("Theme"), centralwidget), 0, 0, 1, 3);
    gridLayout->addWidget(titleEdit, 1, 1, 1, 2);
    gridLayout->addWidget(textEdit, 2, 0, 1, 3);
    gridLayout->addWidget(new QLabel(centralwidget), 3, 0, 1, 2);

    QPushButton *btnSave = new QPushButton(tr("Save"), centralwidget);
    gridLayout->addWidget(btnSave, 3, 2, 1, 1);
    gridLayout->addWidget(new QPushButton(centralwidget), 4, 2, 1, 1);
    gridLayout->addWidget(new QPushButton(tr("New"), centralwidget), 1, 0, 1, 1);
    gridLayout->setColumnStretch(1, 1);
    hbox->addLayout(gridLayout);

    QGridLayout *gridRught = new QGridLayout();
    gridRught->addWidget(new QLabel(centralwidget), 0, 0, 1, 1);

    themesList = new QListView(centralwidget);
    gridRught->addWidget(themesList, 1, 0, 1, 2);
    gridRught->addWidget(new QPushButton(centralwidget), 5, 1, 1, 1);

    QPushButton* btnSync = new QPushButton(centralwidget);
    btnSync->setMaximumSize(QSize(16777215, 50));
    gridRught->addWidget(btnSync, 6, 0, 1, 2);
    gridRught->addWidget(new QLabel(centralwidget), 3, 0, 1, 1);

    attachedRssList = new QListView(centralwidget);
    gridRught->addWidget(attachedRssList, 4, 0, 1, 2);
    gridRught->addWidget(new QPushButton(centralwidget), 2, 0, 1, 2);
    hbox->addLayout(gridRught);

    hbox->setStretch(1, 0);
    setCentralWidget(centralwidget);

    connect(btnSave, SIGNAL(clicked()), this, SLOT(createNode()));
    connect(btnSync, SIGNAL(clicked()), this, SLOT(syncClicked()));
}

void MainWindow::setupDockablePanels()
{
    dock = new QDockWidget(tr("Recent News"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    QWidget *central = new QWidget(dock);

    QBoxLayout *box = new QBoxLayout(QBoxLayout::TopToBottom);
    rssList = new QListView(central);

    box->addWidget(rssList);
    taxThemeList = new QListWidget(central);

    QHBoxLayout *hbox = new QHBoxLayout;

    hbox->addWidget(taxThemeList);
    taxThemeList->setMaximumSize(100, 200);
    taxThemeList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    taxGeoList = new QListWidget(central);
    taxGeoList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    taxGeoList->setMaximumSize(100, 200);
    hbox->addWidget(taxGeoList);
    box->addLayout(hbox);
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

    m_connector = new Connector("http://test.irkipedia.ru/api");
    m_connector->Login("admin", "alcd7c9");
    connect(m_connector, SIGNAL(taxonomyLoaded()), this, SLOT(taxonomyLoaded()));
}

void MainWindow::taxonomyLoaded()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();

    QList<TaxonomyTerm*> themeList = rm->getTaxonomy(ResourceManager::TAXONOMY_THEME);
    if(themeList.size() > 0) {
        taxThemeList->clear();
        for(int i = 0; i < themeList.size(); ++i) {
            QListWidgetItem *item = new QListWidgetItem(themeList[i]->getTitle());
            QVariant data((int)themeList[i]);
            item->setData(Qt::UserRole + 1, data);
            item->setCheckState(Qt::Unchecked);

            taxThemeList->addItem(item);
        }
    }

    QList<TaxonomyTerm*> geoList = rm->getTaxonomy(ResourceManager::TAXONOMY_GEO);
    if(geoList.size() > 0) {
        taxGeoList->clear();
        for(int i = 0; i < themeList.size(); ++i) {
            QListWidgetItem *item = new QListWidgetItem(geoList[i]->getTitle());
            QVariant data((int)geoList[i]);
            item->setData(Qt::UserRole + 1, data);
            item->setCheckState(Qt::Unchecked);

            taxGeoList->addItem(item);
        }
    }
}

void MainWindow::createNode()
{
    QString title = titleEdit->text();
    if(title.isEmpty()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter title of the theme",
                        QMessageBox::Ok, this);
        msg.exec();
        return;
    }

   if(!textEdit->maybeSave()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter content of the theme",
                        QMessageBox::Ok, this);
         msg.exec();
        return;
    }

    QString body = textEdit->getContent();
    Node *n = new Node(title, body);
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    rm->addNode(n);

    titleEdit->clear();
    textEdit->clearContent();
}

void MainWindow::syncClicked()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    m_connector->SyncRss(rm->getUpdatedRss());
}

void MainWindow::rssItemSelected(QModelIndex index)
{
    static QStandardItem *lastItem = NULL;
    RssItem *rss;

    QStandardItemModel *model = static_cast<QStandardItemModel*>(rssList->model());

    QStandardItem *item = model->itemFromIndex(index);

    // apply rss tids
    rss = reinterpret_cast<RssItem*>(item->data().toInt());



     // save  previously selected rss item
    if(item != lastItem) {
        QList<int> tids;

        if(lastItem != NULL) {
            RssItem *rss1 = reinterpret_cast<RssItem*>(lastItem->data().toInt());
            if(rss1) {
                tids = getSelectedTids(taxThemeList);
                tids.append(getSelectedTids(taxGeoList));
                rss1->setTids(tids);
            }
        }
        lastItem = item;
    }

   selectTids(taxThemeList, rss->getTids());
   selectTids(taxGeoList, rss->getTids());
}


void MainWindow::selectTids(QListWidget *widget, const QList<int> &tids)
{
     for(int i = 0; i < widget->count(); ++i) {
         QListWidgetItem *item = widget->item(i);
         TaxonomyTerm* term = reinterpret_cast<TaxonomyTerm*>(item->data(Qt::UserRole + 1).toInt());
         if(tids.indexOf(term->getId()) != -1) {
             item->setCheckState(Qt::Checked);
         }else
             item->setCheckState(Qt::Unchecked);

     }
}

QList<int> MainWindow::getSelectedTids(QListWidget *widget)
{
    QList<int> res;
    for(int i = 0; i < widget->count(); ++i) {

        QListWidgetItem *item = widget->item(i);

        if(item && item->checkState() == Qt::Checked) {
            TaxonomyTerm* term = reinterpret_cast<TaxonomyTerm*>(item->data(Qt::UserRole + 1).toInt());
            if(term) {
                 res.append(term->getId());
            }

        }
    }

    return res;
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
