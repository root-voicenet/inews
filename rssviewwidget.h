#ifndef RSSVIEWWIDGET_H
#define RSSVIEWWIDGET_H

#include <QWidget>

class RssItem;
class TaxonomyWidget;

QT_FORWARD_DECLARE_CLASS(QLabel)
class RssViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RssViewWidget(QWidget *parent = 0);

    void loadRss(RssItem *rss);
    void updateTaxonomy();
    bool storeRss(RssItem *rss);

private: // widgets
    QLabel *titleLabel;
    QLabel *textLabel;
    TaxonomyWidget* taxonomy;
private:
    void setupUI();
signals:

public slots:

};

#endif // RSSVIEWWIDGET_H
