#ifndef CENTERLAWIDGET_H
#define CENTERLAWIDGET_H

#include <QWidget>

class RssViewWidget;
class NodeEditorWidget;
class Node;
class RssItem;

QT_FORWARD_DECLARE_CLASS(QStackedWidget)

class CenterlaWidget : public QWidget
{
    Q_OBJECT
public:
    enum {
        WIDGET_RSS = 0,
        WIDGET_NODE = 1,
        WIDGET_DUMMY = 2
    };
    explicit CenterlaWidget(QWidget *parent = 0);

    void showNode(Node *node);
    void showRss(RssItem* rss);
    void nodeAttachRss(RssItem *rss);
    void showDummy();
private:    // widgets

    RssItem *m_currentRss;
    RssViewWidget* m_rssView;
    NodeEditorWidget *m_nodeView;
    QWidget* m_dummyView;
    QStackedWidget *m_layout;
signals:

public slots:
    void updateTaxonomy();
};

#endif // CENTERLAWIDGET_H
