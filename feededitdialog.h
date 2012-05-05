#ifndef FEEDEDITDIALOG_H
#define FEEDEDITDIALOG_H

#include <QDialog>

namespace Ui {
class FeedEditDialog;
}

QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)

class NvFeedModel;
class NvFeedCategory;
class FeedEditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FeedEditDialog(NvFeedModel *model, NvFeedCategory *selected = NULL, QWidget *parent = 0);
    ~FeedEditDialog();
    
    QString name() const;
    QString url() const;

    QList<NvFeedCategory*> selectedCategories();
private:
    Ui::FeedEditDialog *ui;
    NvFeedCategory *selected_;
    NvFeedModel *model_;

    void refreshTree(NvFeedModel *model, NvFeedCategory *selected);
    QTreeWidgetItem* populateTree(NvFeedCategory *item, NvFeedCategory* selected);
    QList<NvFeedCategory*> _selectedCategories(QTreeWidgetItem *item);

private slots:
    void addCategory_clicked();
};

#endif // FEEDEDITDIALOG_H
