#ifndef RSSITEM_H
#define RSSITEM_H

#include "nvbaseobject.h"

class RssItem : public NvBaseObject
{
public:
    RssItem(int id, const QString &title, int created = 0, const QString &imageUrl = "");
    void setDescription(const QString& desc);
    void setLink(const QString& link);

    QString getImageUrl() const { return m_imageUrl; }
    QString getDescription() const { return m_description; }
    QString getLink() const { return m_link; }
protected:
    QString m_imageUrl;
    QString m_description;
    QString m_link;
};

#endif // RSSITEM_H
