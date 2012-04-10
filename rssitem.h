#ifndef RSSITEM_H
#define RSSITEM_H

#include <QList>
#include <QString>

class RssItem
{
public:
    RssItem(int id, const QString &title, const QString &imageUrl = "");
    void setTids(const QList<int> &tids);

    int getId() const { return m_id; }
    QList<int> &getTids() { return m_tids; }
    QString getTitle() const { return m_title; }
    QString getImageUrl() const { return m_imageUrl; }
private:
    int m_id;
    QList<int> m_tids;
    QString m_title;
    QString m_imageUrl;
};

#endif // RSSITEM_H
