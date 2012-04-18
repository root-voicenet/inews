#ifndef CENTERLAWIDGET_H
#define CENTERLAWIDGET_H

#include <QStackedWidget>

class RssViewWidget;
class NodeEditorWidget;
class Node;
class NvRssItem;

QT_FORWARD_DECLARE_CLASS(QLineEdit)

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);

private: // widgets
    QLineEdit *userEdit, *passwordEdit;

private slots:
    void submitClicked();
};

class CenterlaWidget : public QStackedWidget
{
    friend class LoginWidget;
    Q_OBJECT
public:
    enum {
        WIDGET_RSS = 0,
        WIDGET_NODE = 1,
        WIDGET_DUMMY = 2,
        WIDGET_LOGIN = 3
    };
    explicit CenterlaWidget(QWidget *parent = 0);

    void showNode(Node *node);
    void showRss(NvRssItem* rss);
    void nodeAttachRss(NvRssItem *rss);
    void showDummy();
    void showLogin();
private:    // widgets

    NvRssItem *m_currentRss;
    RssViewWidget* m_rssView;
    NodeEditorWidget *m_nodeView;
    QWidget *m_dummyView, *m_loginView;
private:
    void setLogin(const QString &login, const QString& password);

signals:
    void actionLogin(QString userLogin, QString userPassword);
public slots:
    void updateTaxonomy();
};

#endif // CENTERLAWIDGET_H
