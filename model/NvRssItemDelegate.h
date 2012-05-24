#ifndef NVRSSITEMDELEGATE_H
#define NVRSSITEMDELEGATE_H

#include <QItemDelegate>

class NvRssItemDelegate : public QItemDelegate
{
public:
    NvRssItemDelegate(QObject *parent = 0);
    virtual ~NvRssItemDelegate();

protected:
    // overwrited methods
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index ) const;

    // for tags editor
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,  const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
   // bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private slots:
    void beginCloseEditor();
};
#endif // NVRSSITEMDELEGATE_H
