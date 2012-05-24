#ifndef CENTERLAWIDGET_H
#define CENTERLAWIDGET_H

#include <QWidget>
#include <QFrame>
#include "model/nvrssitem.h"

class RssViewWidget;
class NodeEditorWidget;
class Node;

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QStackedWidget)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSplitter)

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

class CenterlaWidget : public QFrame
{
    friend class LoginWidget;
    Q_OBJECT
public:
    enum {
        WIDGET_DUMMY,
        WIDGET_LOGIN,
        WIDGET_SPLIT,
        WIDGET_RSS,
        WIDGET_NODE
    };

    enum ViewMode {
        VIEW_STACKED,
        VIEW_SPLIT
    };

    explicit CenterlaWidget(QWidget *parent = 0);

    void showNode(const Node &node);
    void newNode();
    void showRss(const NvRssItem &rss);
    void nodeAttachRss(quint32 rss_id);
    void showDummy();
    void showLogin();
    void setViewMode(int mode);

    int currentView() const;
private:    // widgets

    int _viewMode, _prevIndex;
    NvRssItem m_currentRss;
    RssViewWidget* m_rssView;
    NodeEditorWidget *m_nodeView;
    QWidget *m_dummyView, *m_loginView;
    QStackedWidget *stacked;
    QSplitter *splitWidget;
    QPushButton *m_btnNav, *m_btnAttach;
    QLabel *m_titleLabel;
private:
    void setLogin(const QString &login, const QString& password);
    void showLinkTo(int type = 0);
    void showView(int view);
signals:
    void actionLogin(QString userLogin, QString userPassword);

public slots:
    void updateTaxonomy();
    void navigateToOther();
    void attachRss();
    void changeViewMode();
    void attachRss(quint32 rss_id);
};

#endif // CENTERLAWIDGET_H
