#include "NvFeedCategory.h"

int NvFeedCategory::m_maxID = 0;

NvFeedCategory::NvFeedCategory(int id, const QString &title, NvFeedCategory *parent)
    : NvAbstractTreeItem( parent ), m_title( title )
{
    if(!id) {
        m_id = ++m_maxID;
    }else{
        m_id = id;
        m_maxID = qMax(id, m_maxID);
    }

}

int NvFeedCategory::id() const
{
    return m_id;
}

QString NvFeedCategory::title() const
{
    return m_title;
}

void NvFeedCategory::setTitle(const QString &title)
{
    m_title = title;
}

void NvFeedCategory::setId(int id)
{
    m_id = id;
}

QVariant NvFeedCategory::data(int role) const
{
    switch(role) {
        case Qt::DisplayRole:
            return m_title;
            break;
        default:
            return QVariant();
            break;
    }
}

int NvFeedCategory::maxID()
{
    return m_maxID;
}
