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
#include "node.h"

MediaWindow::MediaWindow(WindowManager *wm, QWidget *parent) :
    QMainWindow(parent), m_wm(wm), currentNode(NULL),
    ui(new Ui::MediaWindow)
{
    ui->setupUi(this);

    ui->tabMain->removeTab(1);
    ui->listView->setModel(MediaManager::mediaModel());


    //QObject::connect(ui->treeWidgetFileSystem, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(treeWidget_itemExpanded(QTreeWidgetItem*)));
    //QObject::connect(ui->treeWidgetFileSystem, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeWidget_itemClicked(QTreeWidgetItem*,int)));
    QObject::connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listView_itemDoubleClicked(QModelIndex)));
    QObject::connect(ui->tabMain, SIGNAL(tabCloseRequested(int)), this, SLOT(tabMain_tabCloseRequested(int)));
    QObject::connect(ui->tabMain, SIGNAL(currentChanged(int)), this, SLOT(tabMain_currentChanged(int)));
    //QObject::connect(&model, SIGNAL(needUpdate(QModelIndex)), ui->listView, SLOT(doItemsLayout()));
    QObject::connect(ui->btnUpload, SIGNAL(clicked()), this, SLOT(btnUpload_clicked()));
    QObject::connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(btnRefresh_clicked()));


    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addFileClicked()));
    connect(m_wm->connector(), SIGNAL(mediaLoaded()), this, SLOT(syncComplete()));

    //MediaManager::sync();

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
    MediaManager::select();
}

void MediaWindow::hideEvent(QHideEvent *event)
{
    currentNode = NULL;
    QMainWindow::hideEvent(event);
}


void MediaWindow::closeEvent(QCloseEvent *event)
{
    hide();
}

void MediaWindow::syncComplete()
{
    MediaManager::select();
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
     if(currentNode) {
         NvMediaItem *item = MediaManager::mediaModel()->item(index);
         if(item) {
             currentNode->attachMedia(*item);
         }

         emit fileSelected();
     }
 }

 void MediaWindow::addFileClicked()
 {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Add Image"), QDir::homePath(),
                                                     tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty()) {
        bool bRet = MediaManager::addFile(fileName);
        if( bRet ) {
            ui->btnUpload->setEnabled( true );
        }
    }
 }

 void MediaWindow::btnUpload_clicked()
 {
     MediaManager::upload();
 }

 void MediaWindow::btnRefresh_clicked()
 {
     MediaManager::select();
 }


 void MediaWindow::selectFile(Node *n)
 {
    currentNode = n;
    show();
 }
