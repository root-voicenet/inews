#ifndef LOADERS_H
#define LOADERS_H

#include <QVariant>

class NvRssCachedModel;
class RssImporter
{
public:
    RssImporter(NvRssCachedModel *target);
    bool import(const QVariant &in);

protected:
    NvRssCachedModel *_target;
};



#endif // LOADERS_H
