#ifndef NVNODEMEDIAITEM_H
#define NVNODEMEDIAITEM_H

#include "NvMediaItem.h"

class NvNodeMediaItem : public NvMediaItem
{
public:
    NvNodeMediaItem();
    NvNodeMediaItem(const NvNodeMediaItem &other);
    NvNodeMediaItem(const NvMediaItem& other);
    NvNodeMediaItem& operator = (const NvNodeMediaItem &other);

    QString title() const;
    QString description() const;



    void setTitle(const QString& title);
    void setDescription(const QString& description);
protected:
    QString m_title;
    QString m_description;
};

#endif // NVNODEMEDIAITEM_H
