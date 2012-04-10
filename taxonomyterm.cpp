#include "taxonomyterm.h"

TaxonomyTerm::TaxonomyTerm(int id, const QString &title, TaxonomyTerm *parent)
    : m_id(id), m_title(title), m_parent(parent)
{
}
