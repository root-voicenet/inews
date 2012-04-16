#include "nvbaseobject.h"

NvBaseObject::NvBaseObject(int id, const QString& title, int created)
    : m_id(id), m_title(title), m_created(created), m_tids()
{
}

void NvBaseObject::setTids(const QList<TaxonomyTerm*> &tids)
{
    m_tids = tids;
}

void NvBaseObject::setTitle(const QString &title)
{
    m_title = title;
}
