#include "feededitdialog.h"
#include "model/NvFeedModel.h"
#include "model/NvFeedCategory.h"
#include "ui_feededitdialog.h"
#include "view/NvFeedsTreeView.h"

FeedEditDialog::FeedEditDialog(NvFeedModel *model, NvFeedCategory *selected, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeedEditDialog),
    model_(model)
{
    ui->setupUi(this);
    ui->categoryTree->setColumnCount(1);

    connect(ui->btnNewCat, SIGNAL(clicked()), this, SLOT(addCategory_clicked()));
    refreshTree(model, selected);
}

FeedEditDialog::~FeedEditDialog()
{
    delete ui;
}

void FeedEditDialog::refreshTree(NvFeedModel *model, NvFeedCategory *selected)
{
    ui->categoryTree->clear();

    QTreeWidgetItem *root = ui->categoryTree->invisibleRootItem();
    selected_ = (selected ? selected : model->rootCategory());

    QTreeWidgetItem* item = populateTree(model->rootCategory(), selected);


    root->addChild(item);
}

QString FeedEditDialog::name() const
{
    return ui->nameEdit->text();
}

QString FeedEditDialog::url() const
{
    return ui->urlEdit->text();
}

QTreeWidgetItem* FeedEditDialog::populateTree(NvFeedCategory *item, NvFeedCategory* selected)
{
    QList<QTreeWidgetItem*> res;


    QTreeWidgetItem *treeItem = new QTreeWidgetItem( QStringList(item->name() ));
    treeItem->setData(0, Qt::UserRole +1, (int) item);

    if(item == selected) {
        treeItem->setCheckState(0, Qt::Checked);
    }else{
        treeItem->setCheckState(0, Qt::Unchecked);
    }

    foreach(QObject *c, item->children()) {
        NvFeedCategory *category = qobject_cast<NvFeedCategory*>(c);
        treeItem->addChild(populateTree(category, selected));
    }

    return treeItem;
}

QList<NvFeedCategory*> FeedEditDialog::_selectedCategories(QTreeWidgetItem *item)
{
    QList<NvFeedCategory*> res;
    if(item->checkState(0) == Qt::Checked) {
        res << reinterpret_cast<NvFeedCategory*>(item->data(0, Qt::UserRole + 1).toInt());
    }

    for(int i = 0; i < item->childCount(); ++i) {
        res << _selectedCategories(item->child(i));
    }

    return res;
}

QList<NvFeedCategory*> FeedEditDialog::selectedCategories()
{
    return _selectedCategories(ui->categoryTree->invisibleRootItem());
}


void FeedEditDialog::addCategory_clicked()
{
    Q_ASSERT(selected_);

    if(model_->categoryIsValid(selected_)) {

        QString title = NvFeedsTreeView::askCategoryName(this);
        if(!title.isEmpty()) {
            model_->addCategory(title, selected_);
            refreshTree(model_, selected_);
            ui->categoryTree->expandAll();
        }

    }
}
