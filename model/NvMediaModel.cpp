#include "NvMediaModel.h"
#include "NvMediaItem.h"
#include "resourcemanager.h"
#include "dbmanager.h"
#include "windowmanager.h"
#include "connector.h"
#include "NvNodeMediaItem.h"

#include <QImage>
#include <QIcon>
#include <QPainter>
#include <QtCore>
#include <QMessageBox>
#include <QSqlQuery>
#include <QImageReader>
#include <QSqlError>

const int WIDTH_ICON = 100;
const int HEIGHT_ICON = 90;

const int WIDTH_IMAGE = 90;
const int HEIGHT_IMAGE = 65;

NvMediaModel MediaManager::medias;
QList<QString> MediaManager::_upload;

NvMediaModel::NvMediaModel(QObject *parent)
    : QAbstractListModel(parent)
{
    imagesShow_ = new QFutureWatcher<QImage>(this);
    connect(imagesShow_, SIGNAL(resultReadyAt(int)), SLOT(setItemInList(int)));
    connect(imagesShow_, SIGNAL(finished()), SLOT(finished()));
}

NvMediaModel::~NvMediaModel()
{
    WaitFuture();
    clear();
}

void  NvMediaModel::WaitFuture()
{
    if (imagesShow_->isRunning())
    {
        imagesShow_->cancel();
        imagesShow_->waitForFinished();
    }
}

void NvMediaModel::clear()
{
    qDeleteAll( fileList_ );
    fileList_.clear();
}

QImage prepareImage(NvMediaItem *item)
{
    QImageReader imageReader(item->localPath());
    QSize size;
    int image_width = WIDTH_IMAGE;
    int image_height = HEIGHT_IMAGE;

    if (imageReader.supportsOption(QImageIOHandler::Size))
    {
        size = imageReader.size();
        image_width = size.width();
        image_height = size.height();
    }

    double ratio = (double)image_width / (double)image_height;
    image_height = HEIGHT_IMAGE;
    image_width = ratio * image_height;

    imageReader.setScaledSize(QSize(image_width, image_height));
    QImage image = imageReader.read();



    if (image.isNull())
    {
        QImage imageIcon(QLatin1String(":/images/cover.png"));
        image = imageIcon;
        image = image.scaledToHeight(HEIGHT_IMAGE, Qt::SmoothTransformation);

        image_width = image.width();
        image_height = image.height();
    }


    QRectF target((WIDTH_ICON - image_width) / 2, 0, image_width, image_height);
    QRectF source(0, 0, image_width, image_height);


    QImage imgDraw(QSize(WIDTH_ICON, HEIGHT_ICON), QImage::Format_RGB32);
    QPainter painter(&imgDraw);

    painter.setBrush(Qt::NoBrush);
    painter.fillRect(QRect(0, 0, WIDTH_ICON, HEIGHT_ICON), Qt::white);


    painter.setPen(Qt::black);
    painter.drawImage(target, image, source);
    painter.drawRect(target);

    painter.setPen(Qt::darkBlue);

    QRect rect((WIDTH_ICON - WIDTH_IMAGE) / 2, image_height + 2, WIDTH_IMAGE, HEIGHT_ICON - image_height - 4);
    QFontMetrics font_metrics(painter.font());
    QString elideText = font_metrics.elidedText(item->name(), Qt::ElideRight, WIDTH_IMAGE);
    painter.drawText(rect, Qt::AlignCenter | Qt::ElideRight, elideText);

    return (imgDraw);
}

QPixmap prepareIcon(const QString &name)
{
    QImage imageIcon(QLatin1String(":/images/cover.png"));
    imageIcon = imageIcon.scaledToHeight(HEIGHT_IMAGE, Qt::SmoothTransformation);

    int image_width = imageIcon.width();
    int image_height = imageIcon.height();

    QRectF target((WIDTH_ICON - image_width) / 2, 0, image_width, image_height);
    QRectF source(0, 0, image_width, image_height);


    QPixmap pixDraw(QSize(WIDTH_ICON, HEIGHT_ICON));
    QPainter painter(&pixDraw);

    painter.setBrush(Qt::NoBrush);
    painter.fillRect(QRect(0, 0, WIDTH_ICON, HEIGHT_ICON), Qt::white);


    painter.setPen(Qt::black);
    painter.drawImage(target, imageIcon, source);
    painter.drawRect(target);


    painter.setPen(Qt::darkBlue);

    QRect rect((WIDTH_ICON - image_width) / 2, image_height + 2, image_width, HEIGHT_ICON - image_height - 4);
    QFontMetrics font_metrics(painter.font());
    QString elideText = font_metrics.elidedText(name, Qt::ElideRight, image_width);
    painter.drawText(rect, Qt::AlignCenter | Qt::ElideRight, elideText);

    return pixDraw;
}

void NvMediaModel::setItemInList(int index)
{
    if(index < fileList_.size()) {
        fileList_[index]->setIcon(QIcon(QPixmap::fromImage(imagesShow_->resultAt(index))));
    }

    //emit needUpdate(QModelIndex());
}

void NvMediaModel::finished()
{
    fileList_.clear();
}

/*
bool NvMediaModel::a(const QString &path)
{
    QFileInfo fi(path);

    if(!fi.isFile() || !fi.isReadable())
        return false;

    NvMediaItem *item = new NvMediaItem(0, fi.fileName());
    item->setIcon(prepareIcon(fi.fileName()));
    item->setLocalPath( path );
    //item->setData(Qt::WhatsThisRole, QString(fi.filePath()));
    //addItem(item);
    fileList_.append( item );

    imagesShow_->setFuture(QtConcurrent::mapped(fileList_, prepareImage));

    return true;
}
*/

bool NvMediaModel::add(quint32 fid, const QString &title, const QString &thumbnailUrl)
{
    NvMediaItem *item = new NvMediaItem(fid, title, true);
    item->setIcon(prepareIcon(title));
    item->setNetworkAccessManager( ResourceManager::instance()->getNAM() );

    item->setThumbnailUrl( thumbnailUrl );


   // bool bRet = connect(item, SIGNAL(needUpdate()), this, SIGNAL(needUpdate(QModelIndex)));
   // Q_ASSERT(bRet);

    //addItem(item);
    item->downloadThumbnail();
    int row = rowCount();

    fileList_.append(item);

    return true;
}

QVariant NvMediaModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if (index.row() >= fileList_.size())
        return QVariant();


    if(role == Qt::DisplayRole) {
        return fileList_[index.row()]->name();
    }else if(role == Qt::DecorationRole) {
        return fileList_[index.row()]->icon();
    }

    return QVariant();
}

NvMediaItem *NvMediaModel::item(QModelIndex index)
{
    if(!index.isValid())
        return NULL;

    if (index.row() >= fileList_.size())
        return NULL;

    return fileList_[index.row()];
}

int NvMediaModel::rowCount(const QModelIndex &parent) const
{
    return fileList_.size();
}


QList<NvMediaItem*> NvMediaModel::uploadFiles()
{
    QList<NvMediaItem*> res;
/*
    for(int i = 0; i < items.size(); ++i) {
        NvMediaItem *item = qobject_cast<NvMediaItem*>(items[i]);
        if(item && !item->isRemote()) {
           res << item;
        }
    }
*/
    return res;
}

/*
NvMediaItem *NvMediaModel::media(int id)
{
    /*
    for(int i = 0; i < items.size(); ++i) {
         NvMediaItem *item = qobject_cast<NvMediaItem*>(items[i]);
         if(item && item->id() == id)
             return item;
    }

    return NULL;
}
*/
void MediaManager::init()
{

}

void MediaManager::cleanup()
{

}

NvMediaItem MediaManager::getMedia(quint32 id)
{
    return NvMediaItem(1);
}

bool MediaManager::select(int maxcount)
{
    medias.clear();
    QSqlQuery query(DBManager::instance()->connection());

    QString sql = "select * from " + DBManager::FILES_TABLE;
    if(maxcount) {
        sql = QString("select * from " + DBManager::FILES_TABLE + " limit %1").arg(maxcount);
    }
    if(!query.exec(sql)) {
        qDebug() << "Can fetch files";
        return false;
    }

    int row = medias.rowCount();
    medias.beginInsertRows(QModelIndex(), row, row + query.numRowsAffected());
    while (query.next()) {
        medias.add( query.value(0).toUInt(), query.value(1).toString(), query.value(2).toString() );
    }
    medias.endInsertRows();

    return true;
 }

bool MediaManager::addFile(const QString &path)
{
    _upload.append(path);
    return true;
}

void MediaManager::upload()
{
    Connector *c = WindowManager::instance()->connector();
    if(!_upload.isEmpty()) {
        foreach(QString item, _upload) {
            QFile file(item);
            if(file.exists() && file.open(QFile::ReadOnly)) {
                QList<int> tids;
                QByteArray data = file.readAll();
                QFileInfo fi(file);
                c->UploadFile(&data, fi.completeBaseName(), tids);
            }
        }
        _upload.clear();
    }
}

void MediaManager::sync()
{
    Connector *c = WindowManager::instance()->connector();
    c->GetMedia();
}

bool MediaManager::import(QVariant *resp)
{
    QSqlQuery query(DBManager::instance()->connection());
    QMap<QString, QVariant> media(resp->toMap());

    QList<QVariant> files = media.value("files").toList();
    int op = 0;

    for (int i = 0; i < files.size(); ++i) {
        // parse element
        QMap<QString, QVariant> tags = files[i].toMap();
        QString fileName, thumbnail;
        int fid = 0;


        fileName = tags.value("filename").toString();
        fid = tags.value("fid").toInt();
        if(!fid) {
            qDebug() << "File Id is empty";
            return false;
        }

        if(!tags.value("style_url").isNull()) {
            thumbnail = tags.value("style_url").toString();
        }
        if(tags.contains("op")) {
            QString sop = tags.value("op").toString();
            if(!sop.compare("insert")) {
                op = 1;
            }else if(!sop.compare("update")) {
                op = 2;
            }else if(!sop.compare("delete")) {
                op = 3;
            }
        }

        if(op > 0) {
            if(op == 1) {
                query.prepare("INSERT INTO " + DBManager::FILES_TABLE + " (id, name, url) VALUES(?, ?, ?)" );
                query.addBindValue( fid );
                query.addBindValue( fileName );
                query.addBindValue( thumbnail );
            }else if(op == 2) {
                query.prepare("UPDATE " + DBManager::FILES_TABLE + " SET id=?, name=?, url=? WHERE " );
                query.addBindValue( fid );
                query.addBindValue( fileName );
                query.addBindValue( thumbnail );
            }

            if(!query.exec()) {
                qDebug() << "Cannot store file: ";
                return false;
            }
        }
   }

   return true;
}

void MediaManager::setNodeMedia(quint32 node_id, QList<NvNodeMediaItem> items)
{
    QSqlQuery query(DBManager::instance()->connection());
    query.exec(QString("delete from " + DBManager::NODES_FILES_TABLE + " where node_id = %1").arg(node_id));

    for(int i = 0; i < items.size(); ++i) {
        query.prepare("insert into " + DBManager::NODES_FILES_TABLE + " (file_id, node_id, title, description) VALUES(?, ?, ?, ?)");
        query.addBindValue( items[i].id() );
        query.addBindValue( node_id );
        query.addBindValue( items[i].title() );
        query.addBindValue( items[i].description() );

        if(!query.exec()) {
           qDebug() << "Error executing query: " << query.lastError().driverText();
           break;
        }
    }
}

QList<NvNodeMediaItem> MediaManager::getNodeMedia(quint32 node_id)
{
    QList<NvNodeMediaItem> res;
    QSqlQuery query(DBManager::instance()->connection());

    if(!query.exec(QString("SELECT f.id, f.name, f.url, nf.title, nf.description FROM " + DBManager::NODES_FILES_TABLE + " nf INNER JOIN " + DBManager::FILES_TABLE + " f ON(f.id = nf.file_id) WHERE nf.node_id = %1").arg(node_id))) {
        qDebug() << "Error executing query: " << query.lastError().driverText();
        return res;
    }
    while (query.next()){

        NvNodeMediaItem item(NvMediaItem(query.value(0).toUInt(), query.value(1).toString()));
        item.setThumbnailUrl(query.value(2).toString());
        item.setTitle(query.value(3).toString() );
        item.setDescription(query.value(4).toString());

        res << item;
    }

    return res;
}
