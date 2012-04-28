#include "widgetimage.h"

namespace
{
    const int WIDTH_BUTTON = 40;
    const int HEIGHT_BUTTON = 40;

    enum image_show_size {FIT_IMAGE, REAL_SIZE} im_shz;
}

WidgetImage::WidgetImage(QWidget* parent)
    : QWidget(parent)
{
    current_index_ = -1;

    imageLabel_ = new QLabel(this);
    imageLabel_->setBackgroundRole(QPalette::Dark);
    imageLabel_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel_->setScaledContents(true);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setBackgroundRole(QPalette::Dark);    
    scrollArea_->setVisible(true);
    scrollArea_->setWidget(imageLabel_);



    QIcon icon_left(":Images/NavigationLeft_SelectionSubpicture.png");
    QIcon icon_sizefit(":/Image_2.png");
    QIcon icon_right(":Images/NavigationRight_SelectionSubpicture.png");



    btNext_ = new QToolButton(this);
    btNext_->setIcon(icon_right);
    btNext_->setToolTip("Next image");

    btRealSizeFitImage_ = new QToolButton(this);
    btRealSizeFitImage_->setIcon(icon_sizefit);
    btRealSizeFitImage_->setToolTip("Real Size");

    btPrevious_ = new QToolButton(this);
    btPrevious_->setIcon(icon_left);
    btPrevious_->setToolTip("Previous image");

    connect(btNext_, SIGNAL(clicked()), this, SLOT(btNext_Clicked()));
    connect(btRealSizeFitImage_, SIGNAL(clicked()), this, SLOT(btRealSizeFitImage_Clicked()));
    connect(btPrevious_, SIGNAL(clicked()), this, SLOT(btPrevious_Clicked()));

    this->setAutoFillBackground(true);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    im_shz = FIT_IMAGE;
}




void WidgetImage::SetPixmap(QString fileName)
{
    pixmap_ = QPixmap::fromImage(QImage(fileName));
}


void WidgetImage::SetImage(QString fileImageName, QFileInfoList list, QStatusBar* statusBar, QTabWidget *tabWidget, int index_tab)
{
    tabWidget_ = tabWidget;
    index_tab_ = index_tab;
    statusBar_ = statusBar;
    SetPixmap(fileImageName);    
    list_ = list;

    current_index_ = list_.indexOf(QFileInfo(fileImageName));

    SetEnabledButtons();
}





void WidgetImage::ShowImageEx(bool bRealSize)
{
    if (pixmap_.isNull())
    {
        imageLabel_->setPixmap(pixmap_);
        return;
    }

    bool bWidgetResizable = !bRealSize;


    int im_w = pixmap_.width();
    int im_h = pixmap_.height();

    QRect rect;

    QPixmap pixmap_scalled = pixmap_;

    if (!bRealSize)
    {
        if (im_w > width() || im_h > height() - HEIGHT_BUTTON)
        {
            pixmap_scalled = pixmap_scalled.scaledToHeight(height() - HEIGHT_BUTTON, Qt::SmoothTransformation);

            if (pixmap_scalled.width() > width())
            {
                pixmap_scalled = pixmap_.scaledToWidth(width(), Qt::SmoothTransformation);
            }
        }

        rect = QRect((width() - pixmap_scalled.width()) / 2, (height() - pixmap_scalled.height() - HEIGHT_BUTTON) / 2, pixmap_scalled.width(), pixmap_scalled.height());

        QIcon icon_sizefit(":Images/Image_2.png");
        btRealSizeFitImage_->setIcon(icon_sizefit);
        btRealSizeFitImage_->setToolTip("Real size");
    }
    else
    {
        if (im_w > width() || im_h > height() - HEIGHT_BUTTON)
        {
            int visible_height = height() - HEIGHT_BUTTON;
            double ratio = (double)im_w / (double)im_h;
            int visible_width = visible_height * ratio;
            rect = QRect((width() - visible_width) / 2, (height() - visible_height - HEIGHT_BUTTON) / 2, visible_width, visible_height);                        
        }
        else
        {
            rect = QRect((width() - pixmap_scalled.width()) / 2, (height() - pixmap_scalled.height() - HEIGHT_BUTTON) / 2, pixmap_scalled.width(), pixmap_scalled.height());
            bWidgetResizable = !bWidgetResizable;
        }

        QIcon icon_sizefit(":Images/Image.png");
        btRealSizeFitImage_->setIcon(icon_sizefit);
        btRealSizeFitImage_->setToolTip("Fit to window");
    }

    scrollArea_->setGeometry(rect);
    scrollArea_->setWidgetResizable(bWidgetResizable);
    imageLabel_->setPixmap(pixmap_scalled);
    imageLabel_->resize(QSize(pixmap_scalled.width(), pixmap_scalled.height()));

    imageLabel_->adjustSize();


    tabWidget_->setTabText(index_tab_, list_[current_index_].filePath());
    statusBar_->showMessage(GetInfoAboutImage(list_[current_index_].filePath()));


    scrollArea_->horizontalScrollBar()->setValue(scrollArea_->horizontalScrollBar()->maximum() / 2);
    scrollArea_->verticalScrollBar()->setValue(scrollArea_->verticalScrollBar()->maximum() / 2);

}





void WidgetImage::resizeEvent(QResizeEvent *event)
{
    QRect btPrevious_Rect = QRect((width() / 2) - (1.5 * WIDTH_BUTTON), height() - HEIGHT_BUTTON, WIDTH_BUTTON, HEIGHT_BUTTON);
    QRect btRealSizeFitImage_Rect = QRect((width() / 2) - (0.5* WIDTH_BUTTON), height() - HEIGHT_BUTTON, WIDTH_BUTTON, HEIGHT_BUTTON);
    QRect btNext_Rect = QRect(width() / 2 + (0.5* WIDTH_BUTTON), height() - HEIGHT_BUTTON, WIDTH_BUTTON, HEIGHT_BUTTON);

    btPrevious_->setGeometry(btPrevious_Rect);
    btRealSizeFitImage_->setGeometry(btRealSizeFitImage_Rect);
    btNext_->setGeometry(btNext_Rect);

    ShowImageEx(im_shz == REAL_SIZE);
}


void WidgetImage::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Up:
        case Qt::Key_Right:
            btNext_Clicked();
            break;

        case Qt::Key_Down:
        case Qt::Key_Left:
            btPrevious_Clicked();
            break;

        default:
            QWidget::keyPressEvent(event);
            break;
    }
}

void WidgetImage::SetEnabledButtons()
{
     btNext_->setEnabled(current_index_ + 1 < list_.count());
     btPrevious_->setEnabled(current_index_ - 1 >= 0);
}

void WidgetImage::ReplaceImage()
{
    SetPixmap(list_[current_index_].filePath());

    im_shz = FIT_IMAGE;
    ShowImageEx(im_shz == REAL_SIZE);
    SetEnabledButtons();
}

void WidgetImage::btNext_Clicked()
{
    if (current_index_ + 1 < list_.count())
    {
        ++current_index_;
        ReplaceImage();
    }
}

void WidgetImage::btPrevious_Clicked()
{
    if (current_index_ - 1 >= 0)
    {
        --current_index_;
        ReplaceImage();
    }
}

void WidgetImage::btRealSizeFitImage_Clicked()
{
    if (im_shz == FIT_IMAGE)
    {
        im_shz = REAL_SIZE;
    }
    else
    {
        im_shz = FIT_IMAGE;
    }

    ShowImageEx(im_shz == REAL_SIZE);
}

QString WidgetImage::GetInfoAboutImage(QString fileImage)
{
    QImageReader imageReader(fileImage);
    QSize size;

    int image_width = 0;
    int image_height = 0;

    if (imageReader.supportsOption(QImageIOHandler::Size))
    {
        size = imageReader.size();
        image_width = size.width();
        image_height = size.height();
    }



    QFileInfo fi = QFileInfo(fileImage);
    QDateTime dt = fi.lastModified();


    QString str = "File: " + fi.baseName() + QString(",  size: %1 KB,").arg(fi.size()/1000) +  QString(" image size: %1x").arg(image_width) +  QString("%1 px, ").arg(image_height) + dt.toString("dd.MM.yyyy") + " " + dt.toString("hh:mm");

    return str;
}

void WidgetImage::UpdateStatusBar()
{
    statusBar_->showMessage(GetInfoAboutImage(list_[current_index_].filePath()));
}
