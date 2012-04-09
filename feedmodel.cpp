#include "feedmodel.h"
#include "resourcemanager.h"
#include "newsapplication.h"

FeedModel::FeedModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

bool FeedModel::initFromRPC(QVariant *resp)
{
    QList<QVariant> elements(resp->toList());

   for (int i = 0; i < elements.size(); ++i) {
       // parse element
       QMap<QString, QVariant> tags = elements[i].toMap();
       QString rssTitle;
       QString imageUrl;
       int rssId;

       rssTitle = tags.value("title").toString();
       rssId = tags.value("iid").toInt();
       imageUrl = tags.value("image").toString();

       QStandardItem *item = new QStandardItem(rssTitle);
       item->setCheckable( true );
       if(!imageUrl.isEmpty()) {
           ResourceManager *rm = static_cast<NewsApplication*>(qApp)->getRM();
       }

       item->setData(rssId);

      // m_root->appendRow(item);
   }
    return true;
}
