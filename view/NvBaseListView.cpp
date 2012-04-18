#include "NvBaseListView.h"
#include "../model/NvBaseItemDelegate.h"
#include "../model/NvLineItemDelegate.h"

NvBaseListView::NvBaseListView(QWidget *parent) :
    QTreeView(parent)
{
    _baseDel = new NvBaseItemDelegate(this);
    _lineDel = new NvLineItemDelegate(this);

    setIndentation(0);
    setItemsExpandable(false);
    setRootIsDecorated(false);
    setHeaderHidden(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setMinimumWidth(200);
    setViewMode(VIEW_FULL);
}

NvBaseListView::~NvBaseListView()
{
    delete _baseDel;
    delete _lineDel;
}

void NvBaseListView::setViewMode(int mode)
{
    if(mode == VIEW_FULL)
        setItemDelegate(_baseDel);
    else
        setItemDelegate(_lineDel);
    doItemsLayout();
}

void NvBaseListView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
}
