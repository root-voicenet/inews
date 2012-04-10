#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connector.h"
#include "node.h"
#include "rssitem.h"
#include "taxonomyterm.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(createNode()));
    connect(ui->btnSync, SIGNAL(clicked()), this, SLOT(syncClicked()));
    connect(ui->rssList, SIGNAL(clicked(QModelIndex)), this, SLOT(rssItemSelected(QModelIndex)));

    initWidgets();
}

MainWindow::~MainWindow()
{
    if(m_connector)
        delete m_connector;
    delete ui;
}

void MainWindow::initWidgets()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();

    ui->themesList->setModel(&rm->getThemes());
    ui->rssList->setModel(&rm->getFeed());

    m_connector = new Connector("http://test.irkipedia.ru/api");
    m_connector->Login("admin", "alcd7c9");
    connect(m_connector, SIGNAL(taxonomyLoaded()), this, SLOT(taxonomyLoaded()));
}

void MainWindow::taxonomyLoaded()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();

    QList<TaxonomyTerm*> themeList = rm->getTaxonomy(ResourceManager::TAXONOMY_THEME);
    if(themeList.size() > 0) {
        ui->taxThemeList->clear();
        for(int i = 0; i < themeList.size(); ++i) {
            QListWidgetItem *item = new QListWidgetItem(themeList[i]->getTitle());
            QVariant data((int)themeList[i]);
            item->setData(Qt::UserRole + 1, data);
            item->setCheckState(Qt::Unchecked);

            ui->taxThemeList->addItem(item);
        }
    }

    QList<TaxonomyTerm*> geoList = rm->getTaxonomy(ResourceManager::TAXONOMY_GEO);
    if(geoList.size() > 0) {
        ui->taxGeoList->clear();
        for(int i = 0; i < themeList.size(); ++i) {
            QListWidgetItem *item = new QListWidgetItem(geoList[i]->getTitle());
            QVariant data((int)geoList[i]);
            item->setData(Qt::UserRole + 1, data);
            item->setCheckState(Qt::Unchecked);

            ui->taxGeoList->addItem(item);
        }
    }
}

void MainWindow::createNode()
{
    QString title = ui->titleEdit->text();
    if(title.isEmpty()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter title of the theme",
                        QMessageBox::Ok, this);
        msg.exec();
        return;
    }

   if(!ui->textEdit->maybeSave()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter content of the theme",
                        QMessageBox::Ok, this);
         msg.exec();
        return;
    }

    QString body = ui->textEdit->getContent();
    Node *n = new Node(title, body);
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    rm->addNode(n);

    ui->titleEdit->clear();
    ui->textEdit->clearContent();
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

    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->rssList->model());

    QStandardItem *item = model->itemFromIndex(index);

    // apply rss tids
    rss = reinterpret_cast<RssItem*>(item->data().toInt());



     // save  previously selected rss item
    if(item != lastItem) {
        QList<int> tids;

        if(lastItem != NULL) {
            RssItem *rss1 = reinterpret_cast<RssItem*>(lastItem->data().toInt());
            if(rss1) {
                tids = getSelectedTids(ui->taxThemeList);
                tids.append(getSelectedTids(ui->taxGeoList));
                rss1->setTids(tids);
            }
        }
        lastItem = item;
    }

   selectTids(ui->taxThemeList, rss->getTids());
   selectTids(ui->taxGeoList, rss->getTids());
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
