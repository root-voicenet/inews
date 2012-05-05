#include "NvFeedsTreeView.h"
#include "../resourcemanager.h"
#include "../model/NvFeedModel.h"
#include "../model/NvFeedCategory.h"
#include "../model/NvFeedItem.h"
#include "feededitdialog.h"
#include "windowmanager.h"
#include "connector.h"
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

    m_addFeedAction = new QAction(tr("Add Feed"), this);
    connect(m_addFeedAction, SIGNAL(triggered()), this, SLOT(addFeed()));

    m_delFeedAction = new QAction(tr("Del Feed"), this);
    connect(m_delFeedAction, SIGNAL(triggered()), this, SLOT(delFeed()));

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

QString NvFeedsTreeView::askCategoryName(QWidget *parent, const QString &title)
{
    bool ok;
    QString categoryName = QInputDialog::getText(parent, tr("Category"),
        tr("Category name:"), QLineEdit::Normal, title, &ok);

    return categoryName;
}


void NvFeedsTreeView::showContextMenu(QPoint point)
{
    QList<QAction*> actions;

    QModelIndex index = m_tree->indexAt(point);
    if(!index.isValid()) {
        m_tree->setCurrentIndex(m_model->index(0, 0));
    }


    NvAbstractTreeItem* item = m_model->item(index);
    actions.append(m_addAction);
    actions.append(m_renAction);
    actions.append(m_delAction);
    actions.append(m_addFeedAction);

    if(qobject_cast<NvFeedItem*>(item)) {
        actions.append(m_delFeedAction);
    }



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

            QString title = askCategoryName(this);
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
            QString title = askCategoryName(this);
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

void NvFeedsTreeView::addFeed()
{
    NvFeedCategory* selected = m_model->rootCategory();

    QModelIndex index = m_tree->currentIndex();
    NvAbstractTreeItem *item = m_model->item( index );
    if(qobject_cast<NvFeedCategory*>(item)) {
        selected = qobject_cast<NvFeedCategory*>(item);
    }

    FeedEditDialog dlg(m_model, selected);
    if(dlg.exec() == QDialog::Accepted) {
        QString name = dlg.name();
        QString url = dlg.url();

        if(name.isEmpty() || url.isEmpty()) {
            QMessageBox::warning(this, tr("Type a text"), tr("Please fill all fields in the form"), QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }

        NvFeedItem *feed = new NvFeedItem(NvFeedItem::NEW_FEED_ID, name);
        QList<NvFeedCategory*> categories = dlg.selectedCategories();
        foreach(NvFeedCategory *cat, categories) {
            m_model->addFeed(feed, cat);
        }

        Connector *c = WindowManager::instance()->connector();
        c->EditFeed(Connector::FEED_CREATE, name, url);
        m_model->setPulledFeed(name);

    }
}

void NvFeedsTreeView::delFeed()
{
    QModelIndex index = m_tree->currentIndex();
    NvAbstractTreeItem *item = m_model->item( index );
    NvFeedItem *feed = qobject_cast<NvFeedItem*>(item);
    if(feed && QMessageBox::Yes == QMessageBox::warning(this, tr("Delete Feed"), tr("Are you realy want delete this feed?"), QMessageBox::Yes, QMessageBox::No)) {
        Connector *c = WindowManager::instance()->connector();
        c->EditFeed(Connector::FEED_DELETE, "", "", feed->id());
        m_model->deleteFeed(feed->id());
    }

}
