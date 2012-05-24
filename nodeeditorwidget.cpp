#include "nodeeditorwidget.h"
#include "textedit.h"
#include "node.h"
#include "resourcemanager.h"
#include "model/nvrssitem.h"
#include "model/NvNodeMediaItem.h"
#include "view/TagsWidget.h"
#include "windowmanager.h"
#include "mediawindow.h"
#include "mediatitledialog.h"
#include <QtGui>


NodeEditorWidget::NodeEditorWidget(QWidget *parent) :
    QFrame(parent), m_current(0, "")
{
    setupUI();
}

void NodeEditorWidget::setupUI()
{
    textEdit = new TextEdit(this);
    titleEdit = new QLineEdit(this);
    titleEdit->setObjectName("title-edit");
    setObjectName("node");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPushButton *expandBtn = new QPushButton(this);
    expandBtn->setCheckable( true );
    expandBtn->setIcon(QIcon(":/images/check-dw.png"));
    QPushButton *saveBtn = new QPushButton(tr("Save"), this);
    saveBtn->setIcon(QIcon(":/images/new.png"));

    hidePanel = new QFrame(this);
    QPushButton *btnAddMedia = new QPushButton(tr("Add Media"), hidePanel);

    attachedRssList = new QListWidget(hidePanel);
    attachedMediaList = new QListWidget(hidePanel);
    initListView(attachedRssList);
    initListView(attachedMediaList);
    attachedRssList->setFixedHeight(40);
    summaryEdit = new QTextEdit(hidePanel);
    checkPromoted = new QCheckBox(tr("Promoted"), hidePanel);

    tagsWidget = new TagsWidget(hidePanel);
    QVBoxLayout *pbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    pbox->addWidget(new QLabel("Summary", hidePanel));
    hbox->addWidget(summaryEdit);
    pbox->addLayout(hbox);
    pbox->addWidget(checkPromoted);
    hbox = new QHBoxLayout;
    hbox->addWidget(btnAddMedia);
    hbox->addWidget(attachedMediaList, 1);
    pbox->addLayout(hbox);
    pbox->addWidget(attachedRssList, 1);
    pbox->addWidget(tagsWidget);


    hidePanel->setLayout(pbox);
    hidePanel->hide();

    WindowManager *wm = WindowManager::instance();
    connect(wm->mediaWindow(), SIGNAL(fileSelected()), this, SLOT(updateMedia()));
    connect(expandBtn, SIGNAL(toggled(bool)), hidePanel, SLOT(setVisible(bool)));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(attachedMediaList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(MediaList_doubleClicked(QModelIndex)));
    connect(attachedMediaList, SIGNAL(clicked(QModelIndex)), this, SLOT(MediaList_Clicked(QModelIndex)));
    connect(attachedRssList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(RssList_doubleClicked(QModelIndex)));
    connect(btnAddMedia, SIGNAL(clicked()), this, SLOT(addMedia()));

    //attachedRssList->hide();
    //attachedMediaList = new QListWidget(this);
    //attachedMediaList->hide();
    //attachedMediaList->setMaximumHeight(80);
    //attachedRssList->setMaximumHeight(80);
    //connect(attachedMediaList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(MediaList_doubleClicked(QModelIndex)));

    //tagsWidget = new TagsWidget(this);
    //tagsWidget->hide();


    //summaryEdit->setMaximumHeight(100);
    //summaryEdit->hide();
    QVBoxLayout *vbox = new QVBoxLayout;
    hbox = new QHBoxLayout;
    hbox->addWidget(titleEdit, 1);
    hbox->addWidget(saveBtn);
    hbox->addWidget(expandBtn);
    vbox->addLayout(hbox, 0);
    vbox->addWidget(hidePanel);
    vbox->addWidget(textEdit, 1);

    setLayout(vbox);
}

void NodeEditorWidget::initListView(QListWidget *view)
{
    view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setLayoutMode(QListWidget::Batched);
    view->setFlow(QListWidget::LeftToRight);
    view->setWrapping(true);
    view->setResizeMode(QListWidget::Adjust);
    view->setWordWrap(true);
}

void NodeEditorWidget::clear()
{
    loadNode(Node(0, ""));
}

Node* NodeEditorWidget::currentNode() {
    return &m_current;
}



bool NodeEditorWidget::loadNode(const Node &node)
{
    textEdit->clearContent();
    titleEdit->clear();
    attachedRssList->clear();
    attachedMediaList->clear();
    tagsWidget->clear();
    summaryEdit->clear();


    NvRssCachedModel *rssModel = ResourceManager::instance()->rssModel();

    m_current = node;

    if(m_current.id() > 0) {
        titleEdit->setText(m_current.title());
        summaryEdit->setText(m_current.getSummary());
        textEdit->load(m_current.getBody());
        attachedRssList->clear();

        checkPromoted->setChecked(m_current.promoted());

        // select taxonomy
        tagsWidget->loadTags(m_current.tags());

        updateRss();
        updateMedia();
    }

    return true;
}

void NodeEditorWidget::attachRss(quint32 rss_id)
{
    // check if rss already in list
    for(int i = 0; i < attachedRssList->count(); ++i) {
        QVariant data = attachedRssList->item(i)->data(Qt::UserRole + 1);
        if(!data.isNull()) {
            if(data.toUInt() == rss_id)
                return;
        }
    }

    NvRssCachedModel *rssModel = ResourceManager::instance()->rssModel();
    NvRssItem item = rssModel->search(rss_id);

    QListWidgetItem *listItem = new QListWidgetItem(item.name());
    listItem->setData(Qt::UserRole + 1, item.id());
    attachedRssList->addItem(listItem);
}

void NodeEditorWidget::saveClicked()
{
    QString title = titleEdit->text();
    NvNodeModel *model = ResourceManager::instance()->nodesModel();

    if(title.isEmpty()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter title of the theme",
                        QMessageBox::Ok, this);
        msg.exec();
        return;
    }

    Node *node = NULL;

    node = &m_current;
    node->setStatus(Node::STATUS_COMPOSED);
    node->setTitle(title);

    if(textEdit->maybeSave()) {
        QString body = textEdit->getContent();
        node->setBody(body);
    }

    QString summary = summaryEdit->document()->toPlainText();
    if(!summary.isEmpty()) {
        node->setSummary(summary);
    }

    node->setTags(tagsWidget->selectedTags());
    node->setPromoted( checkPromoted->checkState() == Qt::Checked );

    // check attached rss
    if(attachedRssList->count() > 0) {
        for(int i = 0; i < attachedRssList->count(); ++i) {
            QVariant data = attachedRssList->item(i)->data(Qt::UserRole + 1);
            if(!data.isNull()) {
                node->attachRss(data.toUInt());
            }
        }
    }

    if(m_current.id() > 0) {
        model->save(m_current);
    }else{
        m_current.setDate(QDateTime::currentDateTime());
        model->add(m_current);
    }

    // save all nodes
    model->submitAll();
}

void NodeEditorWidget::updateTaxonomy()
{
    ResourceManager *rm = ResourceManager::instance();
    QTreeWidgetItem* tax = rm->getTaxonomy();
    if(tax->childCount() > 0) {
        //m_taxonomy->loadTaxonomy(tax);
    }
}

void NodeEditorWidget::updateMedia()
{
    QList<NvNodeMediaItem> items = m_current.attachedMedia();
    attachedMediaList->clear();

    foreach(NvMediaItem item, items) {
        QListWidgetItem *listItem = new QListWidgetItem(item.name());
        listItem->setData(Qt::UserRole + 1, (int)item.id());
        attachedMediaList->addItem(listItem);
    }
}

void NodeEditorWidget::updateRss()
{
    QList<quint32> items = m_current.attachedRss();
    attachedRssList->clear();

    NvRssCachedModel *model = ResourceManager::instance()->rssModel();

    foreach(quint32 item, items) {
        NvRssItem rss = model->search(item);
        if(rss.id() > 0) {
            QListWidgetItem *listItem = new QListWidgetItem(rss.name());
            listItem->setData(Qt::UserRole + 1, (int)rss.id());
            attachedRssList->addItem(listItem);
        }

    }
}

void NodeEditorWidget::addMedia()
{
    WindowManager* wm = WindowManager::instance();
    wm->mediaWindow()->selectFile(&m_current);
}

void NodeEditorWidget::delMedia()
{
    QListWidgetItem *item = attachedMediaList->currentItem();
    if(item && m_current.id() > 0) {
        int file_id = item->data(Qt::UserRole + 1).toInt();

        m_current.removeMedia(file_id);
        updateMedia();
    }
}

void NodeEditorWidget::MediaList_doubleClicked(QModelIndex index)
{
    QListWidgetItem *item = attachedMediaList->item(index.row());
    if(item && m_current.id() > 0) {
        int media_id = item->data(Qt::UserRole + 1).toInt();
        if(!media_id)
            return;

        m_current.removeMedia(media_id);
        updateMedia();
    }
}

void NodeEditorWidget::MediaList_Clicked(QModelIndex index)
{
    MediaTitleDialog dlg(this);
    QListWidgetItem *item = attachedMediaList->item(index.row());
    if(item) {
        int media_id = item->data(Qt::UserRole + 1).toInt();
        if(!media_id)
            return;

        NvNodeMediaItem& media = m_current.findAttachedMedia(media_id);
        dlg.setTitle(media.title());
        dlg.setDescription(media.description());

        if(dlg.exec() == QDialog::Accepted) {
            media.setTitle(dlg.getTitle());
            media.setDescription(dlg.getDescription());
        }
    }
}

void NodeEditorWidget::RssList_doubleClicked(QModelIndex index)
{
    QListWidgetItem *item = attachedRssList->item(index.row());
    if(item) {
        int rss_id = item->data(Qt::UserRole + 1).toInt();
        if(!rss_id)
            return;

        m_current.removeAttachedRss(rss_id);
        updateRss();
    }
}
