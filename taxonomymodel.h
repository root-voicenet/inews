#ifndef TAXONOMYMODEL_H
#define TAXONOMYMODEL_H

#include <QStandardItemModel>
// remove later
#include <QStringList>

class TaxonomyModel : public QStandardItemModel
{
    Q_OBJECT

    friend class Connector;
public:
     explicit TaxonomyModel(QObject *parent = 0);

    bool init(const QStringList &lines);
    QList<int> getSelectedPointers();
private:
    bool initFromJSON(const QStringList &lines);
    bool initFromRPC(QVariant *resp);

    QStandardItem *m_root;
};

#endif // TAXONOMYMODEL_H
