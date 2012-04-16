#include "nodeeditorwidget.h"
#include "textedit.h"
#include "node.h"
#include "rssitem.h"
#include "resourcemanager.h"
#include "newsapplication.h"
#include "taxonomywidget.h"
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
    attachedRssList = new QListWidget(this);
    attachedRssList->setMaximumHeight(50);
    m_taxonomy = new TaxonomyWidget(this);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(new QLabel(tr("Theme"), this), 0, 0, 1, 6);
    gridLayout->addWidget(new QLabel(tr("Title"), this), 1, 0, 1, 1);
    gridLayout->addWidget(titleEdit, 1, 1, 1, 5);
    gridLayout->addWidget(textEdit, 2, 0, 1, 6);
    gridLayout->addWidget(new QLabel(tr("Attached RSS"), this), 3, 0, 1, 1);
    gridLayout->addWidget(attachedRssList,  3, 1, 1, 5);
    gridLayout->addWidget(m_taxonomy, 4, 0, 2, 2);

    QPushButton *btnSave = new QPushButton(tr("Save"), this);
    connect(btnSave, SIGNAL(clicked()), this, SLOT(saveClicked()));
    gridLayout->addWidget(btnSave, 5, 5, 1, 1);

    gridLayout->setColumnStretch(1, 1);

    setLayout(gridLayout);
}

void NodeEditorWidget::clear()
{
    m_current = NULL;
}

void NodeEditorWidget::loadNode(Node *node)
{
    if(node && m_current == node)
            return;

    m_taxonomy->clearSelection();
    textEdit->clearContent();
    titleEdit->clear();

    if(node != NULL) {
        titleEdit->setText(node->getTitle());
        textEdit->load(node->getBody());
        attachedRssList->clear();

        // select taxonomy
        if(!node->getTids().isEmpty()) {
            m_taxonomy->selectTaxonomy(node->getTids());
        }

        QList<RssItem*> items = node->attachedRss();
        if(!items.isEmpty()) {
            for(int i = 0; i < items.size(); ++i) {
                QListWidgetItem *listItem = new QListWidgetItem(items[i]->getTitle());
                listItem->setData(Qt::UserRole + 1, (int)items[i]);
                attachedRssList->addItem(listItem);
            }
        }
    }

    m_current = node;
}

void NodeEditorWidget::attachRss(RssItem *rss)
{
    // check if rss already in list
    for(int i = 0; i < attachedRssList->count(); ++i) {
        QVariant data = attachedRssList->item(i)->data(Qt::UserRole + 1);
        if(!data.isNull()) {
            RssItem *currentRss = reinterpret_cast<RssItem*>(data.toInt());
            if(currentRss->getId() == rss->getId())
                return;
        }
    }

    QListWidgetItem *listItem = new QListWidgetItem(rss->getTitle());
    listItem->setData(Qt::UserRole + 1, (int)rss);
    attachedRssList->addItem(listItem);
}

void NodeEditorWidget::saveClicked()
{
    QString title = titleEdit->text();
    if(title.isEmpty()) {
        QMessageBox msg(QMessageBox::Information, "Error", "You must enter title of the theme",
                        QMessageBox::Ok, this);
        msg.exec();
        return;
    }

    Node *node = NULL;
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();

    if(m_current) {
        node = m_current;
        node->setTitle(title);

        if(textEdit->maybeSave()) {
            QString body = textEdit->getContent();
            node->setBody(body);
        }
    }else{
        QString body = textEdit->getContent();
        node = new Node(Node::NODEID_DEFAULT, title, false, 0);
        node->setBody(body);
        rm->addNode( node );
        m_current = node;
    }

    // save taxonomy
    QList<TaxonomyTerm*> tids = m_taxonomy->selectedTaxonomy();
    node->setTids(tids);

    // check attached rss
    if(attachedRssList->count() > 0) {
        for(int i = 0; i < attachedRssList->count(); ++i) {
            QVariant data = attachedRssList->item(i)->data(Qt::UserRole + 1);
            if(!data.isNull()) {
                RssItem *rss = reinterpret_cast<RssItem*>(data.toInt());
                node->attachRss(rss);
            }
        }
    }
    node->setUpdated(true);
}

void NodeEditorWidget::updateTaxonomy()
{
    ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
    QTreeWidgetItem* tax = rm->getTaxonomy();
    if(tax->childCount() > 0) {
        m_taxonomy->loadTaxonomy(tax);
    }
}
