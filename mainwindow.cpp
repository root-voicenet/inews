#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    m_connector = new Connector("http://test.irkipedia.ru/api");
    m_connector->Login("admin", "alcd7c9");
    connect(m_connector, SIGNAL(taxonomyLoaded()), this, SLOT(taxonomyLoaded()));
}

void MainWindow::taxonomyLoaded()
{
    TaxonomyModel *model = NULL;
    model = m_connector->getTaxonomy(1);
    if(model) {
        ui->taxThemeList->setModel(model);
    }

     model = m_connector->getTaxonomy(2);
     if(model) {
         ui->taxGeoList->setModel(model);
     }
}
