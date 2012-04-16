#ifndef TAXONOMYWIDGET_H
#define TAXONOMYWIDGET_H

#include <QWidget>
#include <QList>
#include <QStandardItemModel>

QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)

class TaxonomyTerm;
class TaxonomyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaxonomyWidget(QWidget *parent = 0);

    void loadTaxonomy(QTreeWidgetItem *root);
    void clearSelection();
    void selectTaxonomy(const QList<TaxonomyTerm*> &selected);
    QList<TaxonomyTerm*> selectedTaxonomy();
private:
    QTreeWidget *taxonomyList;

private:
    void setupUI();
    void selectItem(QList<int> *tids, QTreeWidgetItem *parent = 0);
    QList<TaxonomyTerm*> selectedItem(QTreeWidgetItem *parent = 0);

signals:

public slots:

};

#endif // TAXONOMYWIDGET_H
