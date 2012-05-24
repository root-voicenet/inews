#include "loaders.h"
#include "resourcemanager.h"
#include "model/NvRssCachedModel.h"
#include "model/nvrssitem.h"
#include "model/NvFeedModel.h"
#include <QDebug>

RssImporter::RssImporter(NvRssCachedModel *target)
    : _target(target)
{

}

bool RssImporter::import(const QVariant &in)
{
    NvRssCachedModel *rssModel = _target;
    ResourceManager *rm = ResourceManager::instance();

    NvFeedModel *feeds = rm->feedModel();

    if(rssModel) {
        //rssModel->clearRemote();
        QList<QVariant> elements(in.toList());

        for (int i = 0; i < elements.size(); ++i) {
            // parse element
            QMap<QString, QVariant> tags = elements[i].toMap();
            QString rssTitle;
            QUrl imageUrl;

            // create view
            int rssId;
            uint cdate;

            rssTitle = tags.value("title").toString();
            rssId = tags.value("iid").toInt();
            cdate = tags.value("date").toUInt();


            QList<QVariant> images = tags.value("image").toList();
            if(!images.isEmpty()) {
                imageUrl = images.first().toString();
            }

            NvRssItem *rss = new NvRssItem(rssId, rssTitle);

            if(cdate) {
                rss->setDate(QDateTime::fromTime_t( cdate ));
            }

            if(!tags.value("fid").isNull()) {
                quint32 fid = tags.value("fid").toUInt();
                rss->setFeed( feeds->feed( fid ) );
            }

            if(!tags.value("description").isNull()) {
                rss->setDescription(tags.value("description").toString());
            }

            if(!tags.value("tids").isNull()) {
                QList<QVariant> tids = tags.value("tids").toList();
                QList<Tag> res;
                foreach(const QVariant& s, tids) {
                    Tag tag = TagsManager::getTag( s.toUInt() );
                    if(tag.isValid())
                        res << tag;
                }
                rss->setTags(res);
            }

            if(!tags.value("link").isNull()) {
                rss->setLink(tags.value("link").toString());
            }

            if(rssTitle.isEmpty()) {
                qDebug() << "Title is empty";
                delete rss;
                return false;
            }
            //rssModel->addRemote(rss);
        }

        return true;
    }

    return false;
}
