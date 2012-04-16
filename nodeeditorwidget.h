#ifndef NODEEDITORWIDGET_H
#define NODEEDITORWIDGET_H

#include <QWidget>

class TextEdit;
class Node;
class RssItem;
class TaxonomyWidget;

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QListWidget)

class NodeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NodeEditorWidget(QWidget *parent = 0);
    void loadNode(Node *node);
    void attachRss(RssItem *node);
    void clear();
    void updateTaxonomy();
private: //widgets
    TextEdit *textEdit;
    QLineEdit *titleEdit;
    QListWidget *attachedRssList;
    TaxonomyWidget *m_taxonomy;
private:
    Node *m_current;

    void setupUI();
signals:

public slots:
    void saveClicked();
};

#endif // NODEEDITORWIDGET_H
