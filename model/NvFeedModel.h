#ifndef NVFEEDMODEL_H
#define NVFEEDMODEL_H

#include <QStandartItemModel>

class NvFeedModel : public QStandartItemModel
{
    Q_OBJECT
public:
    explicit NvFeedModel(QObject *parent = 0);

signals:

public slots:

};

#endif // NVFEEDMODEL_H
