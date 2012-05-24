#ifndef NODEEDITORWIDGET_H
#define NODEEDITORWIDGET_H

#include <QFrame>
#include <QModelIndex>
#include "node.h"

class TextEdit;
class NvRssItem;
class TagsWidget;

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QFrame)

class NodeEditorWidget : public QFrame
{
    Q_OBJECT
public:
    explicit NodeEditorWidget(QWidget *parent = 0);
    bool loadNode(const Node &node);
    Node *currentNode();
    void attachRss(quint32 rss_id);
    void clear();
    void updateTaxonomy();
private: //widgets
    TextEdit *textEdit;
    QLineEdit *titleEdit;
    QTextEdit *summaryEdit;
    QListWidget *attachedRssList;
    QListWidget *attachedMediaList;
    QCheckBox *checkPromoted;
    TagsWidget *tagsWidget;
    QFrame *hidePanel;
private:
    Node m_current;

    void setupUI();
    void initListView(QListWidget *view);
signals:

public slots:
    void saveClicked();
    void updateMedia();
    void addMedia();
    void delMedia();
    void updateRss();
    void MediaList_doubleClicked(QModelIndex index);
    void MediaList_Clicked(QModelIndex index);
    void RssList_doubleClicked(QModelIndex index);
};

#endif // NODEEDITORWIDGET_H
