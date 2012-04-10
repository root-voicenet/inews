#ifndef TAXONOMYTERM_H
#define TAXONOMYTERM_H

#include <QString>

class TaxonomyTerm
{
public:
    TaxonomyTerm(int id, const QString& title, TaxonomyTerm* parent = NULL);

    int getId() const { return m_id; }
    QString getTitle() const { return m_title; }
    TaxonomyTerm *getParent() { return m_parent; }
private:
    int m_id;
    QString m_title;
    TaxonomyTerm* m_parent;
};

#endif // TAXONOMYTERM_H
