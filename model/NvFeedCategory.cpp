#include "NvFeedCategory.h"

int NvFeedCategory::m_maxID = 0;

NvFeedCategory::NvFeedCategory(int id, const QString &title, NvFeedCategory *parent)
    : NvAbstractTreeItem( 0, title, parent )
{
    if(!id) {
        m_id = ++m_maxID;
    }else{
        m_id = id;
        m_maxID = qMax(id, m_maxID);
    }

}

QVariant NvFeedCategory::data(int role) const
{
    switch(role) {
        case Qt::DisplayRole:
            return m_name;
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

void NvFeedCategory::addFeed(int fid)
{
    m_fids.append(fid);
}
