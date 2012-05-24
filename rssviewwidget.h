#ifndef RSSVIEWWIDGET_H
#define RSSVIEWWIDGET_H

#include <QFrame>

class NvRssItem;
class TagsWidget;

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QWebView)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

class RssViewWidget : public QFrame
{
    Q_OBJECT
public:
    explicit RssViewWidget(QWidget *parent = 0);

    void loadRss(NvRssItem *rss);
    bool storeRss(NvRssItem *rss);
    void showAttachLink(bool show = false);
private: // widgets
    QLabel *titleLabel, *dateLabel, *sourceLabel, *textLabel;
    QWebView *m_browser;
    NvRssItem *m_current;
    QVBoxLayout *vbox;

    TagsWidget* tagsWidget;
private:
    void setupUI();
signals:
    void attachClicked();
private slots:
    void tagsChanged();
    void showHideBrowser();
public slots:

};

#endif // RSSVIEWWIDGET_H
