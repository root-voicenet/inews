#include "NvFeedsTreeView.h"
#include "../resourcemanager.h"
#include "../model/NvFeedModel.h"
#include "../model/NvFeedCategory.h"
#include <QtGui>


NvFeedsTreeView::NvFeedsTreeView(QWidget *parent) :
    QTreeView(parent)
{
    init();
}

void NvFeedsTreeView::init()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    m_model = ResourceManager::instance()->feedModel();
    setModel(m_model);

    m_addAction = new QAction(tr("Add New"), this);
    connect(m_addAction, SIGNAL(triggered()), this, SLOT(addCategory()));

    m_renAction = new QAction(tr("Rename"), this);
    connect(m_renAction, SIGNAL(triggered()), this, SLOT(renameCategory()));

    m_delAction = new QAction(tr("Del category"), this);
    connect(m_delAction, SIGNAL(triggered()), this, SLOT(deleteCategory()));

    // connect custome context menu
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

bool NvFeedsTreeView::askCategoryName(NvFeedCategory *item)
{
    bool ok;
    QString categoryName = QInputDialog::getText(this, tr("Category"),
        tr("Category name:"), QLineEdit::Normal, item->title(), &ok);
    if(ok && !categoryName.isEmpty()) {
        item->setTitle(categoryName);
        return true;
    }

    return false;
}


void NvFeedsTreeView::showContextMenu(QPoint point)
{
    QList<QAction*> actions;

    if(!indexAt(point).isValid()) {
        setCurrentIndex(m_model->index(0, 0));
    }

    actions.append(m_addAction);
    actions.append(m_renAction);
    actions.append(m_delAction);

    if(actions.count() > 0) {
         QMenu::exec(actions, mapToGlobal(point));
    }
}

void NvFeedsTreeView::addCategory()
{
    QModelIndex index = currentIndex();
    if(!index.isValid()) {
        index = m_model->index(0, 0);
    }
    NvAbstractTreeItem *item = m_model->item( index );
    if(dynamic_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *parent = dynamic_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(parent)) {
            NvFeedCategory *newCat = new NvFeedCategory(0, "", parent);

            if(!askCategoryName(newCat)) {
                delete newCat;
                return;
            }

            if(!m_model->saveCategory( newCat )) {

            }
            expand( index );
            doItemsLayout();
        }
    }
}

void NvFeedsTreeView::renameCategory()
{
    QModelIndex index = currentIndex();
    NvAbstractTreeItem *item = m_model->item( index );
    if(dynamic_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *parent = dynamic_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(parent)) {
            if(!askCategoryName(parent) || !m_model->saveCategory( parent )) {

            }
        }
    }
}

void NvFeedsTreeView::deleteCategory()
{
    QModelIndex index = currentIndex();
    NvAbstractTreeItem *item = m_model->item( index );
    if(dynamic_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *sitem = dynamic_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(sitem)) {
            setCurrentIndex( QModelIndex() );
            //sitem->parent()->removeChild( sitem );
            doItemsLayout();
        }
    }

}
