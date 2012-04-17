#include "NvBaseListView.h"
#include "../model/NvBaseItemDelegate.h"

NvBaseListView::NvBaseListView(QWidget *parent) :
    QTreeView(parent)
{
    delegate_ = new NvBaseItemDelegate(this);
    setItemDelegate(delegate_);
    setIndentation(0);
    setItemsExpandable(false);
    setRootIsDecorated(false);
    setHeaderHidden(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setMinimumWidth(200);
}

NvBaseListView::~NvBaseListView()
{
    delete delegate_;
}

void NvBaseListView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
}
