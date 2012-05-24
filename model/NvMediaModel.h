#ifndef NVMEDIAMODEL_H
#define NVMEDIAMODEL_H

#include <QImage>
#include <QFutureWatcher>
#include <QSqlTableModel>

class NvMediaItem;
class NvNodeMediaItem;
class NvMediaModel : public QAbstractListModel
{
    Q_OBJECT
    friend class MediaManager;
public:
    NvMediaModel(QObject *parent = 0);
    virtual ~NvMediaModel();
    void clear();

    QList<NvMediaItem*> uploadFiles();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    NvMediaItem *item(QModelIndex index);
private:
    void WaitFuture();
    bool add(quint32 fid, const QString& title, const QString& thumbnailUrl);

private:
    QList<NvMediaItem*> fileList_;
    QFutureWatcher<QImage> *imagesShow_;

private slots:
    void setItemInList(int index);
    void finished();
};


class MediaManager
{
private:
    static NvMediaModel medias;
    static QList<QString> _upload;
public:
    static void init();
    static void cleanup();

    static NvMediaModel *mediaModel() { return &medias; }

    static NvMediaItem getMedia(quint32 id);
    static bool select(int maxcount = 0);
    static bool addFile(const QString& path);
    static void upload();
    static void sync();
    static bool import(QVariant *resp);
    /**
     * retrive media for nodes
     */
    static QList<NvNodeMediaItem> getNodeMedia(quint32 node_id);
    static void setNodeMedia(quint32 node_id, QList<NvNodeMediaItem> items);
};

QPixmap prepareIcon(const QString &name);
//QPixmap prepareImage(const QFileInfo &infoFile);
QImage prepareImage(NvMediaItem *item);

#endif // NVMEDIAMODEL_H
