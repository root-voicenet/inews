#include "addiconimagethread.h"

#include <QFileSystemModel>
#include <QImage>
#include <QIcon>
#include <QPainter>
#include <QtCore>
#include <QLatin1String>


namespace
{
    const int WIDTH_ICON = 100;
    const int HEIGHT_ICON = 90;

    const int WIDTH_IMAGE = 90;
    const int HEIGHT_IMAGE = 65;
}


AddIconImageThread::AddIconImageThread(QWidget* parent) : QThread(parent), listWidget_(NULL), dirPath_(""), bTerminated_(false)
{
}

AddIconImageThread::~AddIconImageThread()
{

}

void AddIconImageThread::run()
{
    QDir dir(dirPath_);    

    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << ".bmp" << "tiff" << "ico";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList(filters, QDir::Files);
    int count_files = list.count();


    for (int i = 0; i < count_files; ++i)
    {
        mutex_.lock();
        if (bTerminated_)
        {
            this->exit(0);
        }
        mutex_.unlock();


        QString fileName = list[i].filePath();
        QImage image(fileName);

        if (image.isNull())
        {
            QImage imageIcon(QLatin1String(":/Image_2.png"));
            image = imageIcon;
        }

        image = image.scaledToHeight(HEIGHT_IMAGE, Qt::SmoothTransformation);

        int image_width = image.width();
        int image_height = image.height();

        QRectF target((WIDTH_ICON - image_width) / 2, 0, image_width, image_height);
        QRectF source(0, 0, image_width, image_height);

        QPixmap pix(QSize(WIDTH_ICON, HEIGHT_ICON));
        QPainter painter(&pix);

        painter.setBrush(Qt::NoBrush);
        painter.fillRect(QRect(0, 0, WIDTH_ICON, HEIGHT_ICON), Qt::white);


        painter.setPen(Qt::black);
        painter.drawImage(target, image, source);
        painter.drawRect(target);

        painter.setPen(Qt::darkBlue);

        QRect rect((WIDTH_ICON - image_width) / 2, image_height + 2, image_width, HEIGHT_ICON - image_height - 4);
        QFontMetrics font_metrics(painter.font());
        QString elideText = font_metrics.elidedText(list[i].completeBaseName(), Qt::ElideRight, image_width);
        painter.drawText(rect, Qt::AlignCenter | Qt::ElideRight, elideText);

        mutex_.lock();
        if (bTerminated_)
            this->exit(0);
        mutex_.unlock();

        QListWidgetItem *item = listWidget_->item(i);
        item->setIcon(QIcon(pix));        
    }
}


void AddIconImageThread::Start(QString dirPath, QListWidget* list)
{
    dirPath_ = dirPath;
    listWidget_ = list;
    bTerminated_ = false;

    if (!isRunning())
    {
        start(QThread::HighPriority);
    }
}

void AddIconImageThread::Stop()
{
    mutex_.lock();
    bTerminated_ = true;
    mutex_.unlock();
}

