#include "nodeeditorwidget.h"
#include "textedit.h"
#include "node.h"
#include "resourcemanager.h"
#include "taxonomywidget.h"
#include "model/nvrssitem.h"
#include "model/NvNodeMediaItem.h"
#include "windowmanager.h"
#include "mediawindow.h"
#include "mediatitledialog.h"
#include <QtGui>


NodeEditorWidget::NodeEditorWidget(QWidget *parent) :
    QWidget(parent), m_current(NULL)
{
    setupUI();
}

void NodeEditorWidget::setupUI()
{
    textEdit = new TextEdit(this);
    titleEdit = new QLineEdit(this);
    summaryEdit = new QTextEdit(this);

    WindowManager *wm = WindowManager::instance();

    attachedRssList = new QListWidget(this);
    attachedMediaList = new QListWidget(this);
    attachedMediaList->setMaximumHeight(80);
    connect(attachedMediaList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(MediaList_doubleClicked(QModelIndex)));


    attachedRssList->setMaximumHeight(50);
    m_taxonomy = new TaxonomyWidget(this);
    m_taxonomy->setMaximumHeight(100);
    checkPromoted = new QCheckBox(tr("Promoted"), this);

    summaryEdit->setMaximumHeight(100);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(new QLabel(tr("Theme"), this), 0, 0, 1, 6);
    gridLayout->addWidget(new QLabel(tr("Title"), this), 1, 0, 1, 1);
    gridLayout->addWidget(titleEdit, 1, 1, 1, 5);
    gridLayout->addWidget(new QLabel(tr("Summary"), this), 2, 0, 1, 1);
    gridLayout->addWidget(summaryEdit, 2, 1, 1, 6);
    gridLayout->addWidget(textEdit, 3, 0, 1, 6);
    gridLayout->addWidget(new QLabel(tr("Attached RSS"), this), 4, 0, 1, 1);
    gridLayout->addWidget(attachedRssList,  4, 1, 1, 5);
    gridLayout->addWidget(checkPromoted, 5, 0, 1, 2);

    gridLayout->addWidget(new QLabel(tr("Tags"), this), 6, 0, 1, 2);
    gridLayout->addWidget(m_taxonomy, 7, 0, 1, 2);
    gridLayout->addWidget(new QLabel(tr("Media"), this), 6, 2, 1, 4);

    QHBoxLayout *hb = new QHBoxLayout;
    QToolButton *tbD = new QToolButton(this);
    tbD->setText(tr("D"));
    QToolButton *tbA = new QToolButton(this);
    tbA->setText(tr("A"));
    hb->addWidget(tbD);
    hb->addWidget(tbA);
    hb->addStretch(1);
    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(attachedMediaList);
    vb->addLayout(hb);

    connect(tbA, SIGNAL(clicked()), this, SLOT(addMedia()));
    connect(tbD, SIGNAL(clicked()), this, SLOT(delMedia()));

    gridLayout->addLayout(vb, 7, 2, 1, 4);

    QPushButton *btnSave = new QPushButton(tr("Save"), this);
    connect(btnSave, SIGNAL(clicked()), this, SLOT(saveClicked()));
    gridLayout->addWidget(btnSave, 8, 5, 1, 1);

    gridLayout->setColumnStretch(1, 1);

    setLayout(gridLayout);
}

void NodeEditorWidget::clear()
{
    m_current = NULL;
}

Node* NodeEditorWidget::loadNode(Node *node)
{
    if(node && m_current == node)
            return m_current;

    m_taxonomy->clearSelection();
    textEdit->clearContent();
    titleEdit->clear();

    m_current = node;

    if(node != NULL) {
        titleEdit->setText(node->getTitle());
        summaryEdit->setText(node->getSummary());
        textEdit->load(node->getBody());
        attachedRssList->clear();

        checkPromoted->setChecked(node->promoted());

        // select taxonomy

        if(!node->getTids().isEmpty()) {
            m_taxonomy->selectTaxonomy(node->getTids());
        }


        QList<NvRssItem*> items = node->attachedRss();
        if(!items.isEmpty()) {
            for(int i = 0; i < items.size(); ++i) {
                QListWidgetItem *listItem = new QListWidgetItem(items[i]->name());
                listItem->setData(Qt::UserRole + 1, (int)items[i]);
                attachedRssList->addItem(listItem);
            }
        }

        updateMedia();
    }else{
        ResourceManager *rm = ResourceManager::instance();
        node = new Node(Node::NODEID_DEFAULT, "new node", false, 0);
        rm->addNode( node, true );
    }

    WindowManager *wm = WindowManager::instance();
    connect(wm->mediaWindow(), SIGNAL(fileSelected()), this, SLOT(updateMedia()));


    return node;
}

void NodeEditorWidget::attachRss(NvRssItem *rss)
{
    // check if rss already in list
    for(int i = 0; i < attachedRssList->count(); ++i) {
        QVariant data = attachedRssList->item(i)->data(Qt::UserRole + 1);
        if(!data.isNull()) {
            NvRssItem *currentRss = reinterpret_cast<NvRssItem*>(data.toInt());
            if(currentRss->id() == rss->id())
                return;
        }
    }

    QListWidgetItem *listItem = new QListWidgetItem(rss->name());
    listItem->setData(Qt::UserRole + 1, (int)rss);
    attachedRssList->addItem(listItem);
}

void NodeEditorWidget::saveClicked()
{
    Q_ASSERT(m_current);

    QString title = titleEdit->text();
    if(title.isEmpty()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter title of the theme",
                        QMessageBox::Ok, this);
        msg.exec();
        return;
    }

    Node *node = NULL;

    node = m_current;
    node->setTitle(title);

    if(textEdit->maybeSave()) {
        QString body = textEdit->getContent();
        node->setBody(body);
    }

    QString summary = summaryEdit->document()->toPlainText();
    if(!summary.isEmpty()) {
        node->setSummary(summary);
    }

    // save taxonomy
    node->setTids(m_taxonomy->selectedTaxonomy());

    node->setPromoted( checkPromoted->checkState() == Qt::Checked );

    // check attached rss
    if(attachedRssList->count() > 0) {
        for(int i = 0; i < attachedRssList->count(); ++i) {
            QVariant data = attachedRssList->item(i)->data(Qt::UserRole + 1);
            if(!data.isNull()) {
                NvRssItem *rss = reinterpret_cast<NvRssItem*>(data.toInt());
                node->attachRss(rss);
            }
        }
    }
    node->setUpdated(true);
}

void NodeEditorWidget::updateTaxonomy()
{
    ResourceManager *rm = ResourceManager::instance();
    QTreeWidgetItem* tax = rm->getTaxonomy();
    if(tax->childCount() > 0) {
        m_taxonomy->loadTaxonomy(tax);
    }
}

void NodeEditorWidget::updateMedia()
{
    QList<NvNodeMediaItem> items = m_current->attachedMedia();
    attachedMediaList->clear();

    foreach(NvMediaItem item, items) {\
        QListWidgetItem *listItem = new QListWidgetItem(item.name());
        listItem->setData(Qt::UserRole + 1, (int)item.id());
        attachedMediaList->addItem(listItem);
    }
}


void NodeEditorWidget::addMedia()
{
    WindowManager* wm = WindowManager::instance();
    wm->mediaWindow()->selectFile(m_current);
    m_current->setUpdated(true);
}

void NodeEditorWidget::delMedia()
{
    QListWidgetItem *item = attachedMediaList->currentItem();
    if(item && m_current) {
        int file_id = item->data(Qt::UserRole + 1).toInt();

        m_current->removeMedia(file_id);
        m_current->setUpdated(true);
        updateMedia();
    }
}

void NodeEditorWidget::MediaList_doubleClicked(QModelIndex index)
{
    MediaTitleDialog dlg(this);
    QListWidgetItem *item = attachedMediaList->item(index.row());
    if(item && m_current) {
        int media_id = item->data(Qt::UserRole + 1).toInt();
        if(!media_id)
            return;

        NvNodeMediaItem& media = m_current->findAttachedMedia(media_id);
        dlg.setTitle(media.title());
        dlg.setDescription(media.description());

        if(dlg.exec() == QDialog::Accepted) {
            media.setTitle(dlg.getTitle());
            media.setDescription(dlg.getDescription());
        }
    }
}
