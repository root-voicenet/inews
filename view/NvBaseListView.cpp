#include "NvBaseListView.h"
#include "../model/NvBaseItemDelegate.h"

NvBaseListView::NvBaseListView(QWidget *parent) :
    QListView(parent)
{
    _baseDel = new NvBaseItemDelegate(this);
   // _lineDel = new NvLineItemDelegate(this);

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setMinimumWidth(200);
    setViewMode(VIEW_FULL);
}

NvBaseListView::~NvBaseListView()
{
    delete _baseDel;
    //delete _lineDel;
}

void NvBaseListView::setViewMode(int mode)
{
    if(mode == VIEW_FULL)
        setItemDelegate(_baseDel);
    //else
    //    setItemDelegate(_lineDel);
    //doItemsLayout();
}

void NvBaseListView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);
}
