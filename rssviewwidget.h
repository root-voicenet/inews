#ifndef RSSVIEWWIDGET_H
#define RSSVIEWWIDGET_H

#include <QWidget>

class NvRssItem;
class TaxonomyWidget;

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QWebView)
class RssViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RssViewWidget(QWidget *parent = 0);

    void loadRss(NvRssItem *rss);
    void updateTaxonomy();
    bool storeRss(NvRssItem *rss);

private: // widgets
    QLabel *titleLabel;
    QLabel *textLabel;
    QWebView *m_browser;

    TaxonomyWidget* taxonomy;
private:
    void setupUI();
signals:

public slots:

};

#endif // RSSVIEWWIDGET_H
