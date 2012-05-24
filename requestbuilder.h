#ifndef REQUESTBUILDER_H
#define REQUESTBUILDER_H

#include "xmlrpc/variant.h"
#include <QList>
#include "model/nvrssitem.h"


class NvRssCachedModel;
class Node;

class RequestBuilder
{
public:
    static bool buildSyncRss(xmlrpc::Variant *request,  QList<NvRssItem> &rss);
    static bool buildSyncNodes(xmlrpc::Variant *request, QList<Node> nodes);
};

#endif // REQUESTBUILDER_H
