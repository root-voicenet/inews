#ifndef NVRSSMODEL_H
#define NVRSSMODEL_H

#include <QSqlRelationalTableModel>

class NvRssModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:

    enum RssDataRole
    {
        FeedRole = Qt::UserRole + 100,
        DateRole,
        DescriptionRole,
        TagRole,
        FeedIdRole
    };

    explicit NvRssModel(QObject *parent = 0);
protected: // overwrited
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    

signals:
    
public slots:
    
};

#endif // NVRSSMODEL_H
