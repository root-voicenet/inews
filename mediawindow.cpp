#include "mediawindow.h"
#include "ui_mediawindow.h"
#include "model/NvMediaItem.h"

#include <QFileSystemModel>
#include <QImage>
#include <QIcon>
#include <QPainter>
#include <QtCore>
#include <QMessageBox>

#include <QImageReader>
#include <QFileIconProvider>
#include <QPushButton>
#include <QFileDialog>

#include "widgetimage.h"
#include "windowmanager.h"
#include "connector.h"

MediaWindow::MediaWindow(WindowManager *wm, QWidget *parent) :
    QMainWindow(parent), m_wm(wm),
    ui(new Ui::MediaWindow)
{
    ui->setupUi(this);

    ui->tabMain->removeTab(1);
    ui->listView->setModel(&model);


    //QObject::connect(ui->treeWidgetFileSystem, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(treeWidget_itemExpanded(QTreeWidgetItem*)));
    //QObject::connect(ui->treeWidgetFileSystem, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeWidget_itemClicked(QTreeWidgetItem*,int)));
    QObject::connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listView_itemDoubleClicked(QModelIndex)));
    QObject::connect(ui->tabMain, SIGNAL(tabCloseRequested(int)), this, SLOT(tabMain_tabCloseRequested(int)));
    QObject::connect(ui->tabMain, SIGNAL(currentChanged(int)), this, SLOT(tabMain_currentChanged(int)));
    QObject::connect(&model, SIGNAL(needUpdate(QModelIndex)), ui->listView, SLOT(doItemsLayout()));
    QObject::connect(ui->btnUpload, SIGNAL(clicked()), this, SLOT(btnUpload_clicked()));
    QObject::connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(btnRefresh_clicked()));


    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addFileClicked()));

    //AddItemToTreeWidgetFileSystem();

    QStatusBar *bar = new QStatusBar(this);
    this->setStatusBar(bar);
    statusBar()->showMessage(tr("Ready"));
}

MediaWindow::~MediaWindow()
{
    delete ui;
}

void MediaWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MediaWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent( event );

    Connector* c = m_wm->connector();
    c->GetMedia();

}


void MediaWindow::closeEvent(QCloseEvent *event)
{

}

/*
void MediaWindow::ShowImageIconFile(QString dirPath)
{   
    WaitFuture();   
    ui->listWidget->clear();
    fileList_ = getListFiles(dirPath);
    int count = fileList_.count();

    for (int i = 0; i < count; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(prepareIcon(fileList_[i])));
        item->setData(Qt::WhatsThisRole, QString(fileList_[i].filePath()));        
        ui->listWidget->addItem(item);
    }

    imagesShow_->setFuture(QtConcurrent::mapped(fileList_, prepareImage));
}
*/



/*
void MediaWindow::AddItemToTreeWidgetFileSystem()
{
    QFileInfoList listDrives = QDir::drives();
    int count_drives = listDrives.count();

    QTreeWidgetItem *parentItem = ui->treeWidgetFileSystem->invisibleRootItem();

    for (int i = 0; i < count_drives; ++i)
    {
        if (listDrives[i].isReadable())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            QString strDrv = listDrives[i].filePath();
            strDrv.remove(strDrv.length() - 1, 1);
            item->setText(0, strDrv);
            item->setIcon(0, QFileIconProvider().icon(listDrives[i]));
            item->setData(0, Qt::WhatsThisRole, listDrives[i].absoluteFilePath());

            QTreeWidgetItem *subitem = new QTreeWidgetItem;
            subitem->setData(0, Qt::WhatsThisRole, QString("@@@"));
            subitem->setText(0, listDrives[i].filePath());
            item->addChild(subitem);
            parentItem->addChild(item);
        }
    }
}
*/

/*
 void MediaWindow::treeWidget_itemExpanded(QTreeWidgetItem *item)
 {
     int child_count = item->childCount();
     QString data = item->child(0)->data(0, Qt::WhatsThisRole).toString();

     if (child_count == 1 && data == QString("@@@"))
     {
         QString strPath = item->child(0)->text(0);
         QDir dir(strPath);

         item->takeChild(0);

         QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
         int count_dirs = dirList.count();

         for (int i = 0; i < count_dirs; ++i)
         {
             QTreeWidgetItem *subitem = new QTreeWidgetItem;
             QString strDrv = dirList[i].fileName();
             subitem->setText(0, strDrv);
             subitem->setIcon(0, QFileIconProvider().icon(dirList[i]));
             subitem->setData(0, Qt::WhatsThisRole, dirList[i].absoluteFilePath());

             QDir dirSub(dirList[i].filePath());
             QFileInfoList dirSubList = dirSub.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

             if (dirSubList.count() > 0)
             {
                QTreeWidgetItem *subsubitem = new QTreeWidgetItem;
                subsubitem->setData(0, Qt::WhatsThisRole, QString("@@@"));
                subsubitem->setText(0, dirList[i].filePath());
                subitem->addChild(subsubitem);
             }

             item->addChild(subitem);
         }
     }
 }


 void MediaWindow::treeWidget_itemClicked(QTreeWidgetItem *item, int column)
 {
     QString strDirPath = item->data(0, Qt::WhatsThisRole).toString();

     if (strDirPath != lastClickedDir_)
     {
        ShowImageIconFile(strDirPath);
     }

     lastClickedDir_ = strDirPath;

     this->setWindowTitle(lastClickedDir_);
 }
*/


 void MediaWindow::tabMain_tabCloseRequested(int index_tab)
 {
     if (index_tab == 0)
         this->close();

     ui->tabMain->removeTab(index_tab);
 }


void MediaWindow::tabMain_currentChanged(int index_tab)
{
    if (index_tab == 0)
    {
        statusBar()->showMessage(tr("Ready"));
    }
    else
    {
        WidgetImage* img = qobject_cast<WidgetImage*>(ui->tabMain->widget(index_tab));
        if(img)
            img->UpdateStatusBar();
    }
}

 QFileInfoList MediaWindow::getListFiles(QString dirPath) const
 {
     QDir dir(dirPath);

     QStringList filters;
     filters << "*.jpg" << "*.jpeg" << "*.png" << ".bmp" << "tiff" << "ico";
     dir.setNameFilters(filters);

     return dir.entryInfoList(filters, QDir::Files);
 }

 void MediaWindow::listView_itemDoubleClicked(QModelIndex index)
 {
     //QString strDirPath = item->data(Qt::WhatsThisRole).toString();

     //WidgetImage *widget = new WidgetImage();
     //widget->setGeometry(0, 0, ui->tabMain->width(), ui->tabMain->height());
     //widget->SetImage(strDirPath, fileList_, this->statusBar(), ui->tabMain, ui->tabMain->count());

    // ui->tabMain->addTab(widget, strDirPath);
     //ui->tabMain->setCurrentWidget(widget);
 }

 void MediaWindow::addFileClicked()
 {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Add Image"), QDir::homePath(),
                                                     tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty()) {
        bool bRet = model.addFile(fileName);
        if( bRet ) {
            ui->btnUpload->setEnabled( true );
        }
    }
 }

 void MediaWindow::btnUpload_clicked()
 {
    Connector *c = m_wm->connector();
    QList<NvMediaItem*> files = model.uploadFiles();
    if(!files.isEmpty()) {
        foreach(NvMediaItem *item, files) {
            QFile file(item->localPath());
            if(file.exists() && file.open(QFile::ReadOnly)) {
                QList<int> tids;
                QByteArray data = file.readAll();
                c->UploadFile(&data, item->name(), tids);
            }
        }
    }
 }

 void MediaWindow::btnRefresh_clicked()
 {
     Connector *c = m_wm->connector();
     c->GetMedia();
 }

 bool MediaWindow::parseRemoteFiles(QVariant *result)
 {
     QList<QVariant> elements(result->toList());
     model.clear();


     for (int i = 0; i < elements.size(); ++i) {
         // parse element
         QMap<QString, QVariant> tags = elements[i].toMap();
         QString fileName, thumbnail;
         int fid = 0;


         fileName = tags.value("filename").toString();
         fid = tags.value("fid").toInt();
         if(!fid) {
             qDebug() << "File Id is empty";
             model.clear();
             return false;
         }

         if(!tags.value("style_url").isNull()) {
             thumbnail = tags.value("style_url").toString();
         }

         model.addRemoteFile(fid, fileName, thumbnail);
     }

     return true;
 }
