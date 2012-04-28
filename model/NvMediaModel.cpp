#include "NvMediaModel.h"
#include "NvMediaItem.h"
#include "resourcemanager.h"

#include <QImage>
#include <QIcon>
#include <QPainter>
#include <QtCore>
#include <QMessageBox>

#include <QImageReader>

const int WIDTH_ICON = 100;
const int HEIGHT_ICON = 90;

const int WIDTH_IMAGE = 90;
const int HEIGHT_IMAGE = 65;

NvMediaModel::NvMediaModel(QObject *parent)
    : NvObjectModel(parent)
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

    emit needUpdate(QModelIndex());
}

void NvMediaModel::finished()
{
    fileList_.clear();
}

bool NvMediaModel::addFile(const QString &path)
{
    QFileInfo fi(path);

    if(!fi.isFile() || !fi.isReadable())
        return false;

    NvMediaItem *item = new NvMediaItem(0, fi.fileName());
    item->setIcon(prepareIcon(fi.fileName()));
    item->setLocalPath( path );
    //item->setData(Qt::WhatsThisRole, QString(fi.filePath()));
    addItem(item);
    fileList_.append( item );

    imagesShow_->setFuture(QtConcurrent::mapped(fileList_, prepareImage));

    return true;
}

bool NvMediaModel::addRemoteFile(quint32 fid, const QString &title, const QString &thumbnailUrl)
{
    NvMediaItem *item = new NvMediaItem(fid, title, true);
    item->setIcon(prepareIcon(title));
    item->setNetworkAccessManager( ResourceManager::instance()->getNAM() );

    item->setThumbnailUrl( thumbnailUrl );
   // bool bRet = connect(item, SIGNAL(needUpdate()), this, SIGNAL(needUpdate(QModelIndex)));
   // Q_ASSERT(bRet);

    addItem(item);
    item->downloadThumbnail();
}

QVariant NvMediaModel::itemData(int row, int role) const
{
    if(role == Qt::BackgroundRole) {
        NvMediaItem *item = qobject_cast<NvMediaItem*>( items[row] );
        if(item && !item->isRemote()) {
            return Qt::gray;
        }
    }

    return NvObjectModel::itemData( row, role );
}

QList<NvMediaItem*> NvMediaModel::uploadFiles()
{
    QList<NvMediaItem*> res;

    for(int i = 0; i < items.size(); ++i) {
        NvMediaItem *item = qobject_cast<NvMediaItem*>(items[i]);
        if(item && !item->isRemote()) {
           res << item;
        }
    }

    return res;
}
