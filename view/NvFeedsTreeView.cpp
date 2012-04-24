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
    setHeaderHidden( true );

    m_addAction = new QAction(tr("Add New"), this);
    connect(m_addAction, SIGNAL(triggered()), this, SLOT(addCategory()));

    m_renAction = new QAction(tr("Rename"), this);
    connect(m_renAction, SIGNAL(triggered()), this, SLOT(renameCategory()));

    m_delAction = new QAction(tr("Del category"), this);
    connect(m_delAction, SIGNAL(triggered()), this, SLOT(deleteCategory()));

    // connect custome context menu
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

QString NvFeedsTreeView::askCategoryName(const QString& title)
{
    bool ok;
    QString categoryName = QInputDialog::getText(this, tr("Category"),
        tr("Category name:"), QLineEdit::Normal, title, &ok);

    return categoryName;
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
    if(qobject_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *parent = qobject_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(parent)) {

            QString title = askCategoryName();
            if(!title.isEmpty()) {
                m_model->addCategory(title, parent);
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
    if(qobject_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *parent = qobject_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(parent)) {
            QString title = askCategoryName();
            if(!title.isEmpty()) {
                m_model->saveCategory( parent );
            }
        }
    }
}

void NvFeedsTreeView::deleteCategory()
{
    QModelIndex index = currentIndex();
    NvAbstractTreeItem *item = m_model->item( index );
    if(qobject_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *sitem = qobject_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(sitem)) {
            setCurrentIndex( QModelIndex() );
            //sitem->parent()->removeChild( sitem );
            doItemsLayout();
        }
    }

}
