#ifndef CENTERLAWIDGET_H
#define CENTERLAWIDGET_H

#include <QWidget>

class RssViewWidget;
class NodeEditorWidget;
class Node;
class NvRssItem;

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)

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

class CenterlaWidget : public QWidget
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
    void newNode();
    void showRss(NvRssItem* rss);
    void nodeAttachRss(NvRssItem *rss);
    void showDummy();
    void showLogin();
private:    // widgets

    NvRssItem *m_currentRss;
    Node *m_currentNode;

    RssViewWidget* m_rssView;
    NodeEditorWidget *m_nodeView;
    QWidget *m_dummyView, *m_loginView;
    QStackedWidget *stacked;
    QPushButton *m_btnNav, *m_btnAttach;
    QLabel *m_titleLabel;
private:
    void setLogin(const QString &login, const QString& password);
    void showLinkTo(int type = 0);
signals:
    void actionLogin(QString userLogin, QString userPassword);

public slots:
    void updateTaxonomy();
    void navigateToOther();
    void attachRss();
    void attachRss(NvRssItem *item);
};

#endif // CENTERLAWIDGET_H
