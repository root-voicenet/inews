#include "NvRssItem.h"

NvRssItem::NvRssItem( quint32 id, const QString & name /*= QString()*/, const QString & desc /*= QString()*/ )
{
	name_ = name;
	description_ = desc;
	updated_ = false;
    id_ = id;
}

quint32 NvRssItem::id() const {
    return id_;
}

void NvRssItem::setId(quint32 id)
{
    id_ = id;
}

QString NvRssItem::name() const
{
	return name_;
}

void NvRssItem::setName( const QString &v )
{
	name_ = v;
}

QString NvRssItem::description() const
{
	return description_;
}

void NvRssItem::setDescription( const QString &v )
{
	description_ = v;
}

QString NvRssItem::source() const
{
	return source_;
}

void NvRssItem::setSource( const QString &v )
{
	source_ = v;
}

QImage NvRssItem::icon() const
{
	return icon_;
}

void NvRssItem::setIcon( const QImage &v )
{
	icon_ = v;
}

QDate NvRssItem::date() const
{
	return date_;
}

void NvRssItem::setDate( const QDate &v )
{
	date_ = v;
}


void NvRssItem::setHasUpdate( bool v )
{
	updated_ = v;
}
