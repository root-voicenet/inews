#ifndef NVMEDIAMODEL_H
#define NVMEDIAMODEL_H

#include "NvObjectModel.h"

#include <QImage>
#include <QFutureWatcher>

class NvMediaItem;
class NvMediaModel : public NvObjectModel
{
    Q_OBJECT
public:
    NvMediaModel(QObject *parent = 0);
    virtual ~NvMediaModel();

    bool addFile(const QString& path);
    bool addRemoteFile(quint32 fid, const QString& title, const QString& thumbnailUrl);
    QList<NvMediaItem*> uploadFiles();
    NvMediaItem *media(int id);
protected:
    virtual QVariant itemData(int row, int role) const;
private:
    void WaitFuture();
private:
    QList<NvMediaItem*> fileList_;
    QFutureWatcher<QImage> *imagesShow_;

private slots:
    void setItemInList(int index);
    void finished();
};

QPixmap prepareIcon(const QString &name);
//QPixmap prepareImage(const QFileInfo &infoFile);
QImage prepareImage(NvMediaItem *item);

#endif // NVMEDIAMODEL_H
