#ifndef NVRSSCACHEDMODEL_H
#define NVRSSCACHEDMODEL_H

#include "nvrssitem.h".h"
#include "Tag.h"
#include <QSqlRelationalTableModel>

class DBManager;
class NvRssItem;

class NvRssCachedModel : public QSqlRelationalTableModel
{
public:

    enum RssDataRole
    {
        FeedRole = Qt::UserRole + 100,
        DateRole,
        DescriptionRole,
        TagRole,
        FeedIdRole,
        RssIdRole,
        ReadedRole
    };

    enum RssImportOperation {
        OP_UNKNOWN = 0,
        OP_INSERT,
        OP_UPDATE,
        OP_DELETE
    };

    NvRssCachedModel(QObject *parent = 0);

    bool import(const QVariant *in);

    NvRssItem item(const QModelIndex &index) const;
    NvRssItem search(quint32 id) const;

    bool save(const NvRssItem& item);
    QList<Tag> populateTags(int id);
    QList<NvRssItem> updatedItems() const;
    void clearUpdated();

public: // overwrited
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QList<Tag> parseTags(const QVariant *input);
    int parseOperation(const QString& opstring ) const;

    DBManager *m_storage;
    QList<NvRssItem> _updated;
};

#endif // NVRSSCACHEDMODEL_H
