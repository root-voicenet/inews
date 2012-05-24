#include "TagsWidget.h"
#include <QComboBox>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTreeView>
#include <QHeaderView>
#include <QMouseEvent>

#include "model/Tag.h"

NvTagsCombobox::NvTagsCombobox(QWidget* button, QWidget *parent)
    : _button(button), QComboBox(parent), skipNextHide(false)
{
    QTreeView* v = new QTreeView(this);
    v->header()->hide();
    v->viewport()->installEventFilter(this);
    setView(v);
}

bool NvTagsCombobox::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && object == view()->viewport())
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = view()->indexAt(mouseEvent->pos());
        if (!view()->visualRect(index).contains(mouseEvent->pos()))
            skipNextHide = true;
    }
    return false;
}

void NvTagsCombobox::setModel(QAbstractItemModel *model)
{
    QComboBox::setModel(model);
    QTreeView *v = qobject_cast<QTreeView*>(view());
    v->expandAll();
}

void NvTagsCombobox::showPopup()
{
    setRootModelIndex(QModelIndex());
    QTreeView *v = qobject_cast<QTreeView*>(view());
    v->expandAll();

    QComboBox::showPopup();
    QPoint pos = mapToGlobal(_button->pos());

    v->window()->move(QPoint(pos.x(), pos.y() - (v->window()->height() + 20)));
}

void NvTagsCombobox::hidePopup()
{
    setRootModelIndex(view()->currentIndex().parent());
    setCurrentIndex(view()->currentIndex().row());
    if (skipNextHide)
        skipNextHide = false;
    else
        QComboBox::hidePopup();
}

void CloseLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent( ev );

    if(ev->button() == Qt::LeftButton)
        emit clicked();
}

TagWidget::TagWidget(const QString &name, Tag *tag, QWidget *parent)
    : QFrame(parent), _tag(tag)
{
    setObjectName("tag");
    setFixedHeight(16);
    QHBoxLayout *lay = new QHBoxLayout;
    QToolButton *closeBtn = new QToolButton(this);
    closeBtn->setIcon(QIcon(":/images/x.png"));
    connect(closeBtn, SIGNAL(clicked()), this, SIGNAL(clicked()));
    QLabel *label = new QLabel(name, this);
    label->setObjectName("tag-label");

    lay->setMargin(0);
    lay->addWidget(label);
    lay->addWidget(closeBtn);

    setLayout(lay);
}

QLabel *TagWidget::createCloseLabel()
{
    CloseLabel *label = new CloseLabel;
    label->setEnabled(true);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    label->setBackgroundRole(QPalette::Base);
    label->setMinimumSize(16, 16);
    label->setPixmap(QPixmap::fromImage(QImage(":/images/x.png")));

    return label;
}



TagsWidget::TagsWidget(QWidget *parent) :
    QFrame(parent)
{
    TagsManager::init();
    setObjectName("tags");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    bt_ = new QToolButton(this);
    bt_->setIcon(QIcon(":/images/tag.png"));
    combo_ = new NvTagsCombobox(bt_, this);
    combo_->hide();

    QHBoxLayout *mainLay = new QHBoxLayout;
    lay = new QHBoxLayout;

    connect(bt_, SIGNAL(clicked()), this, SLOT(addTagClicked()));
    connect(combo_, SIGNAL(activated(int)), this, SLOT(tagSelected(int)));

    bt_->adjustSize();
    combo_->setModel(TagsManager::knownTagsModel());

    mainLay->setMargin(0);
    mainLay->addWidget(bt_);
    mainLay->addLayout(lay);
    mainLay->addStretch(1);
    //mainLay->addWidget(combo_);

    setLayout(mainLay);
}

TagsWidget::~TagsWidget()
{
    destroy();
}

QSize TagsWidget::sizeHint() const
{
    return QSize(parentWidget()->width(), combo_->height());
}

QWidget *TagsWidget::createTag(const QString &name, Tag* tag)
{
    TagWidget *widget = new TagWidget(name, tag, this);
    connect(widget, SIGNAL(clicked()), this, SLOT(removeTag()));

    return widget;
}

void TagsWidget::removeTag()
{
    TagWidget *tag = qobject_cast<TagWidget*>(sender());
    _tags.removeOne(*tag->tag());

    /*
    for(int i = 0; i < lay->count(); ++i) {
        QLayoutItem *item = lay->takeAt(i);
        if(item->widget() && item->widget() == tag) {
            _tags.removeAt(i);
        }
    }*/

    disconnect(tag, SIGNAL(clicked()), this, SLOT(removeTag()));
    tag->deleteLater();

    emit changed();
}

void TagsWidget::addTagClicked()
{
    combo_->showPopup();

    adjustSize();
}

void TagsWidget::tagSelected(int row)
{
    combo_->hidePopup();

    QString name = combo_->itemText(row);
    Tag tag = TagsManager::getTag( name );
    if(tag.isValid() && !_tags.contains(tag)) {
         _tags << tag;
        lay->addWidget(createTag(name, &_tags.last()));


        emit changed();
    }

    adjustSize();
}

void TagsWidget::clear()
{
   clearLayout(lay);

    _tags.clear();
}

bool TagsWidget::loadTags(QList<Tag> tags)
{
    clear();
    for(int i = 0; i < tags.size(); ++i) {
        Tag tag = TagsManager::getTag( tags[i].id() );
        if(tag.isValid()) {
            _tags << tag;
           lay->addWidget(createTag(tag.name(), &_tags.last()));
        }
    }

    return true;
}

QList<Tag> TagsWidget::selectedTags() const
{
    return _tags;
}

void TagsWidget::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}
