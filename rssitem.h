#ifndef RSSITEM_H
#define RSSITEM_H

#include <QList>
#include <QString>

class TaxonomyTerm;
class RssItem
{
public:
    RssItem(int id, const QString &title, const QString &imageUrl = "");
    void setTids(const QList<TaxonomyTerm*> &tids);

    int getId() const { return m_id; }
    QList<TaxonomyTerm*> &getTids() { return m_tids; }
    QString getTitle() const { return m_title; }
    QString getImageUrl() const { return m_imageUrl; }
private:
    int m_id;
    QList<TaxonomyTerm*> m_tids;
    QString m_title;
    QString m_imageUrl;
};

#endif // RSSITEM_H
