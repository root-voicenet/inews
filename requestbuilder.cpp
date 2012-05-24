#include "requestbuilder.h"
#include "model/NvRssCachedModel.h"
#include "model/nvrssitem.h"
#include "model/NvMediaItem.h"
#include "node.h"

#include <qtextdocument.h>

bool RequestBuilder::buildSyncRss(xmlrpc::Variant *request, QList<NvRssItem> &rss)
{
    Q_ASSERT(request);

    QList<xmlrpc::Variant> items;

    for(int i = 0; i < rss.size(); ++i) {
      QMap<QString, xmlrpc::Variant> resItem;
      resItem.insert("id", rss[i].id());

      QList<Tag> tags = rss[i].tags();
      QList<xmlrpc::Variant> resTids;

      for(int j = 0; j < tags.size(); ++j)
          resTids.append(tags[j].id());

          resItem.insert("tids", resTids);

          items << resItem;
      }

    *request = items;

    return true;
}

bool RequestBuilder::buildSyncNodes(xmlrpc::Variant *request, QList<Node> nodes)
{
   QList<xmlrpc::Variant> res;
   for(int i = 0; i < nodes.size(); ++i) {

       QMap<QString, xmlrpc::Variant> nodeItem;
       QString action = "update";
       QList<xmlrpc::Variant> rss;
       if(nodes[i].id() == Node::NODEID_DEFAULT) {
           action = "create";
       }
       nodeItem.insert("action", action);
       nodeItem.insert("id", nodes[i].id());
       nodeItem.insert("title", nodes[i].title());
       nodeItem.insert("summary", nodes[i].getSummary());
       nodeItem.insert("promoted", nodes[i].promoted() ? 1 : 0);


       if(!nodes[i].getBody().isEmpty()) {
           nodeItem.insert("body", Qt::escape(nodes[i].getBody()));
       }

       // attach tids
       QList<Tag> tags = nodes[i].tags();
       QList<xmlrpc::Variant> nodeTids;

       for(int j = 0; j < tags.size(); ++j)
           nodeTids.append(tags[j].id());

       nodeItem.insert("tids", nodeTids);

       // attach rss ids
       QList<quint32> attached = nodes[i].attachedRss();
       for(int j = 0; j < attached.size(); ++j)
           rss << attached[j];

       if(rss.size() > 0) {
           nodeItem.insert("rss", rss);
       }

       // attach files
       QList<xmlrpc::Variant> files;
       QList<NvNodeMediaItem> medias = nodes[i].attachedMedia();
       for(int j = 0; j < medias.size(); ++j) {
            QMap<QString, xmlrpc::Variant> fileData;
            fileData.insert("fid", medias[j].id());
            fileData.insert("title", medias[j].title());
            fileData.insert("description", medias[j].description());
            files << fileData;
       }

       if(files.size() > 0) {
           nodeItem.insert("files", files);
       }

       res.append(nodeItem);
   }

   *request = res;
   return true;
}
