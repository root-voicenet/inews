#ifndef WIDGETIMAGE_H
#define WIDGETIMAGE_H


#include <QtGui>

class WidgetImage : public QWidget
{
    Q_OBJECT

public:
    WidgetImage(QWidget* parent = 0);

    void SetImage(QString fileImageName, QFileInfoList list, QStatusBar* statusBar, QTabWidget *tabWidget, int index_tab);
    void UpdateStatusBar();

protected:    
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void ShowImageEx(bool bRealSize = false);
    void SetPixmap(QString fileName);
    void SetEnabledButtons();
    void ReplaceImage();
    QString GetInfoAboutImage(QString fileImage);

private slots:
    void btNext_Clicked();
    void btPrevious_Clicked();
    void btRealSizeFitImage_Clicked();


private:
    QScrollArea *scrollArea_;
    QPixmap pixmap_;
    QFileInfoList list_;
    QLabel *imageLabel_;    
    QToolButton *btNext_;
    QToolButton *btRealSizeFitImage_;
    QToolButton *btPrevious_;
    QStatusBar* statusBar_;
    QTabWidget *tabWidget_;
    int index_tab_;
    int current_index_;
    QRect btNext_Rect_;
    QRect btPrevious_Rect_;


};

#endif // WIDGETIMAGE_H
