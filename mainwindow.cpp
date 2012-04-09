#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connector.h"
#include "node.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(createNode()));

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

    m_connector = new Connector("http://test.irkipedia.ru/api");
    m_connector->Login("admin", "alcd7c9");
    connect(m_connector, SIGNAL(taxonomyLoaded()), this, SLOT(taxonomyLoaded()));
}

void MainWindow::taxonomyLoaded()
{
    TaxonomyModel *model = NULL;
    model = m_connector->getTaxonomy(1);
    if(model) {
      //  ui->taxThemeList->setModel(model);
    }

     model = m_connector->getTaxonomy(2);
     if(model) {
     //    ui->taxGeoList->setModel(model);
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
