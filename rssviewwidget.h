#ifndef RSSVIEWWIDGET_H
#define RSSVIEWWIDGET_H

#include <QWidget>

class NvRssItem;
class TaxonomyWidget;

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QWebView)
QT_FORWARD_DECLARE_CLASS(QCheckBox)

class RssViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RssViewWidget(QWidget *parent = 0);

    void loadRss(NvRssItem *rss);
    void updateTaxonomy();
    bool storeRss(NvRssItem *rss);
    void showAttachLink(bool show = false);
private: // widgets
    QLabel *titleLabel;
    QLabel *textLabel;
    QLabel *attachLink;
    QWebView *m_browser;
    QCheckBox *checkPromoted;

    TaxonomyWidget* taxonomy;
private:
    void setupUI();
signals:
    void attachClicked();
public slots:

};

#endif // RSSVIEWWIDGET_H
