#ifndef REQUESTBUILDER_H
#define REQUESTBUILDER_H

#include "xmlrpc/variant.h"
#include <QList>


class NvRssCachedModel;
class Node;

class RequestBuilder
{
public:
    static bool buildSyncRss(xmlrpc::Variant *request, NvRssCachedModel *model);
    static bool buildSyncNodes(xmlrpc::Variant *request, QList<Node*> nodes);
};

#endif // REQUESTBUILDER_H
