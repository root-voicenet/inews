#ifndef NVOBJECTIMPORTER_H
#define NVOBJECTIMPORTER_H

#include <QVariant>

class NvObjectModel;
class NvObjectImporter
{
public:
    NvObjectImporter(NvObjectModel *target) : m_target(target) {}
    virtual ~NvObjectImporter() {};

    virtual bool import(const QVariant& in) = 0;

protected:
    NvObjectModel *m_target;
};

#endif // NVOBJECTIMPORTER_H
