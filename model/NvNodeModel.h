#ifndef NVNODEMODEL_H
#define NVNODEMODEL_H

#include <QSqlTableModel>
#include "../node.h"

class NvNodeModel : public QSqlTableModel
{
    Q_OBJECT
public:

    enum NodeDataRole
    {
        DateRole = Qt::UserRole + 100,
        NodeIDRole,
        DescriptionRole,
        TagRole,
        StatusRole
    };

    enum NodeImportOperation {
        OP_UNKNOWN = 0,
        OP_INSERT,
        OP_UPDATE,
        OP_DELETE
    };

    enum SortMode {
        SORT_DATE_ASC,
        SORT_DATE_DSC,
        SORT_TITLE_ASC,
        SORT_TITLE_DSC
    };

   explicit NvNodeModel(QObject *parent = 0);

    bool import(const QVariant *in);

    Node item(const QModelIndex &index) const;
    Node search(quint32 node_id) const;
    bool save(const Node& item);
    bool add(const Node& node);
    QList<Tag> populateTags(int id);
    QList<Node> composedItems() const;
    void clearUpdated();

    void setSortMode(int sortMode);

public: // overwrited
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QList<Tag> parseTags(const QVariant *input);
    int parseOperation(const QString& opstring ) const;

    void setNodeRss(quint32 node_id, QList<quint32> items);
    QList<quint32> getNodeRss(quint32 node_id);

    QList<Node> _updated;

signals:
    
public slots:
    
};

#endif // NVNODEMODEL_H
