#include "NvBaseListView.h"
#include "../model/NvRssItemDelegate.h".h"
#include <QAction>
#include <QMenu>
#include "../model/NvRssCachedModel.h"
#include "../model/nvrssitem.h"

NvBaseListView::NvBaseListView(QWidget *parent) :
    QListView(parent)
{
    _baseDel = new NvRssItemDelegate(this);
   // _lineDel = new NvLineItemDelegate(this);
    m_attachAction = new QAction(tr("Attach Rss"), this);
    connect(m_attachAction, SIGNAL(triggered()), this, SLOT(attachRss()));

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::DoubleClicked
                                | QAbstractItemView::SelectedClicked);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    setMinimumWidth(200);
    setViewMode(VIEW_FULL);

}

void NvBaseListView::showContextMenu(QPoint point)
{
    QList<QAction*> actions;

    if(indexAt(point).isValid()) {
        actions.append(m_attachAction);
    }

    if(actions.count() > 0) {
         QMenu::exec(actions, mapToGlobal(point));
    }
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

void NvBaseListView::attachRss()
{
    QModelIndex index = currentIndex();
    NvRssCachedModel *m = dynamic_cast<NvRssCachedModel*>(model());

    if(m && index.isValid()) {
        NvRssItem item = m->item(index);
        if(item.id() != 0) {
            emit attachSelected(item.id());
        }
    }
}
