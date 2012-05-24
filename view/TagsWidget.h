#ifndef TAGSWIDGET_H
#define TAGSWIDGET_H

#include <QFrame>
#include <QList>
#include <QComboBox>
#include <QLabel>
#include "../model/Tag.h"

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QToolButton)
QT_FORWARD_DECLARE_CLASS(QStandardItemModel)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)

class NvTagsCombobox : public QComboBox
{
public:
    NvTagsCombobox(QWidget *button, QWidget* parent = 0);
    bool eventFilter(QObject* object, QEvent* event);
    void setModel(QAbstractItemModel *model);

    virtual void showPopup();
    virtual void hidePopup();
private:
    bool skipNextHide;
    QWidget *_button;
};

class CloseLabel : public QLabel
{
    Q_OBJECT
public:
    CloseLabel(QWidget *parent = 0) : QLabel(parent) {}

signals:
    void clicked();

public:
    void mousePressEvent(QMouseEvent *ev);
};

class TagWidget : public QFrame
{
    Q_OBJECT
public:
    TagWidget(const QString& name, Tag* tag, QWidget *parent = 0);
    QLabel *createCloseLabel();

    Tag* tag() const { return _tag; }

private:
    Tag* _tag;
signals:
    void clicked();
    void editingFinished();
};

class TagsWidget : public QFrame
{
    Q_OBJECT
public:
    explicit TagsWidget(QWidget *parent = 0);
    ~TagsWidget();
    void addTag(const QString& tagname);
    
    QSize sizeHint() const;
    void clear();

    bool loadTags(QList<Tag> tags);
    QList<Tag> selectedTags() const;
private:
    QList<Tag> _tags;
    NvTagsCombobox *combo_;
    QToolButton *bt_;
    QHBoxLayout *lay;
    QStandardItemModel *model;


    QWidget *createTag(const QString &name, Tag *tag);

signals:
    void changed();
    void editingFinished();


private slots:
    void addTagClicked();
    void tagSelected(int row);
    void removeTag();
    void clearLayout(QLayout *layout);
};

#endif // TAGSWIDGET_H
