#ifndef FEEDMODEL_H
#define FEEDMODEL_H

#include <QStandardItemModel>

class FeedModel : public QStandardItemModel
{
    Q_OBJECT

    friend class Connector;
public:
    explicit FeedModel(QObject *parent = 0);

private:
    bool initFromRPC(QVariant *resp);

signals:
    
public slots:
    
};

#endif // FEEDMODEL_H
