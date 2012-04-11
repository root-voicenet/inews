#ifndef TAXONOMYWIDGET_H
#define TAXONOMYWIDGET_H

#include <QWidget>
#include <QList>

QT_FORWARD_DECLARE_CLASS(QListWidget)
QT_FORWARD_DECLARE_CLASS(QListWidget)

class TaxonomyTerm;
class TaxonomyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaxonomyWidget(QWidget *parent = 0);

    void loadThemeTaxonomy(const QList<TaxonomyTerm*> &terms);
    void loadGeoTaxonomy(const QList<TaxonomyTerm*> &terms);
    void clearSelection();
    void selectTaxonomy(const QList<TaxonomyTerm*> &selected);
    QList<TaxonomyTerm*> selectedTaxonomy();
private:
    QListWidget *themeList;
    QListWidget *geoList;

private:
    void loadTaxonomy(QListWidget *widget, const QList<TaxonomyTerm*> &terms);
    void clearSelection(QListWidget *widget);
    void selectTaxonomy(QListWidget *widget, const QList<TaxonomyTerm*> &terms);
    QList<TaxonomyTerm*> selectedTaxonomy(QListWidget *widget);
    void setupUI();

signals:

public slots:

};

#endif // TAXONOMYWIDGET_H
