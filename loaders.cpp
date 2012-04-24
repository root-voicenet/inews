#include "loaders.h"
#include "resourcemanager.h"
#include "model/NvRssCachedModel.h"
#include "model/nvrssitem.h"
#include <QDebug>

RssImporter::RssImporter(NvObjectModel *target)
    : NvObjectImporter(target)
{

}

bool RssImporter::import(const QVariant &in)
{
    NvRssCachedModel *rssModel = dynamic_cast<NvRssCachedModel*>(m_target);
    if(rssModel) {
        rssModel->clearRemote();
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

            NvRemoteRssItem *rss = new NvRemoteRssItem(rssId, rssTitle);

            if(cdate) {
                rss->setDate(QDateTime::fromTime_t( cdate ));
            }

            if(!tags.value("sourse").isNull()) {
                rss->setSource(tags.value("sourse").toString());
            }

            if(!tags.value("description").isNull()) {
                rss->setDescription(tags.value("description").toString());
            }

            rss->setPromoted( tags.value("promoted").toInt() == 1 );

            if(!tags.value("tids").isNull()) {
                QList<QVariant> tids = tags.value("tids").toList();
                QList<int> res;
                foreach(const QVariant& i, tids) {
                    res << qvariant_cast<int>(i);
                }
                rss->setTerms(res);
            }

            if(!tags.value("link").isNull()) {
                rss->setLink(tags.value("link").toString());
            }

            if(rssTitle.isEmpty()) {
                qDebug() << "Title is empty";
                delete rss;
                return false;
            }
            rssModel->addRemote(rss);
        }

        return true;
    }

    return false;
}
