#ifndef NVBASEOBJECT_H
#define NVBASEOBJECT_H



#include <QList>
#include <QString>

class TaxonomyTerm;

class NvBaseObject
{
public:
    NvBaseObject(int id, const QString& title, int created = 0);

    void setTids(const QList<TaxonomyTerm*> &tids);
    void setTitle(const QString& title);

    int getId() const { return m_id; }
    int getCreated() const { return m_created; }
    QList<TaxonomyTerm*> &getTids() { return m_tids; }
    QString getTitle() const { return m_title; }

protected:
    QList<TaxonomyTerm*> m_tids;
    int m_created;
    int m_id;
    QString m_title;
};

#endif // NVBASEOBJECT_H
