#ifndef LOADERS_H
#define LOADERS_H

#include "model/NvObjectImporter.h"

class RssImporter : public NvObjectImporter
{
public:
    RssImporter(NvObjectModel *target);
    bool import(const QVariant &in);
};

#endif // LOADERS_H
