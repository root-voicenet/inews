#include "NvFeedsTreeView.h"
#include "../resourcemanager.h"
#include "../model/NvFeedModel.h"
#include "../model/NvFeedCategory.h"
#include "../model/NvFeedItem.h"
#include <QtGui>
#include <QTreeView>


NvFeedsTreeView::NvFeedsTreeView(QWidget *parent) :
    QWidget(parent)
{
    init();
}

void NvFeedsTreeView::init()
{
    m_model = ResourceManager::instance()->feedModel();

    m_tree = new QTreeView( this );
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tree->setModel(m_model);
    m_tree->setHeaderHidden( true );
    m_tree->setDragEnabled( true );
    m_tree->setAcceptDrops( true );

    m_addAction = new QAction(tr("Add New"), this);
    connect(m_addAction, SIGNAL(triggered()), this, SLOT(addCategory()));

    m_renAction = new QAction(tr("Rename"), this);
    connect(m_renAction, SIGNAL(triggered()), this, SLOT(renameCategory()));

    m_delAction = new QAction(tr("Del category"), this);
    connect(m_delAction, SIGNAL(triggered()), this, SLOT(deleteCategory()));

    // connect custome context menu
    connect(m_tree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(m_tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    m_btnClear = new QPushButton(tr("Clear"), this);
    m_btnClear->hide();
    connect(m_btnClear, SIGNAL(clicked()), this, SLOT(clearFilter()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin( 0 );
    vbox->addWidget(m_btnClear);
    vbox->addWidget(m_tree, 1);
    setLayout(vbox);
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

    if(!m_tree->indexAt(point).isValid()) {
        m_tree->setCurrentIndex(m_model->index(0, 0));
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
    QModelIndex index = m_tree->currentIndex();
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

            m_tree->expand( index );
            m_tree->doItemsLayout();
        }
    }
}

void NvFeedsTreeView::renameCategory()
{
    QModelIndex index = m_tree->currentIndex();
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
    QModelIndex index = m_tree->currentIndex();
    NvAbstractTreeItem *item = m_model->item( index );
    if(qobject_cast<NvFeedCategory*>(item)) {
        NvFeedCategory *sitem = qobject_cast<NvFeedCategory*>(item);
        if(m_model->categoryIsValid(sitem)) {
            m_tree->setCurrentIndex( QModelIndex() );
            //sitem->parent()->removeChild( sitem );
            m_tree->doItemsLayout();
        }
    }

}

void NvFeedsTreeView::itemClicked(QModelIndex index)
{
    if(!index.isValid())
        return;

    NvFeedItem *item = qobject_cast<NvFeedItem*>(m_model->item(index));
    if(item) {
        m_btnClear->show();
        emit feedClicked( item->id() );
    }
}

void NvFeedsTreeView::clearFilter()
{
    m_btnClear->hide();
    emit feedClicked( 0 );
}
