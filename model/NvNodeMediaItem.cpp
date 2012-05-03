#include "NvNodeMediaItem.h"

NvNodeMediaItem::NvNodeMediaItem()
    : NvMediaItem(0, "", false)
{

}

NvNodeMediaItem::NvNodeMediaItem(const NvNodeMediaItem& other)
    : NvMediaItem(other.id_, other.filename_, other.is_remote)
{
    *this = other;
}

NvNodeMediaItem::NvNodeMediaItem(const NvMediaItem &other)
    : NvMediaItem(other)
{

}

NvNodeMediaItem& NvNodeMediaItem::operator =(const NvNodeMediaItem& other)
{
    *((NvMediaItem*)this) = *((NvMediaItem*) &other);
    m_title = other.m_title;
    m_description = other.m_description;
}


QString NvNodeMediaItem::title() const
{
    return m_title;
}

QString NvNodeMediaItem::description() const
{
    return m_description;
}

void NvNodeMediaItem::setTitle(const QString &title)
{
    m_title = title;
}

void NvNodeMediaItem::setDescription(const QString &description)
{
    m_description = description;
}

