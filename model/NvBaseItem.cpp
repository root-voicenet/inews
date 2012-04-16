#include "NvBaseItem.h"

NvBaseItem::NvBaseItem( quint32 id, const QString & name /*= QString()*/, const QString & desc /*= QString()*/ )
{
	name_ = name;
	description_ = desc;
	updated_ = false;
    id_ = id;
}

quint32 NvBaseItem::id() const {
    return id_;
}

void NvBaseItem::setId(quint32 id)
{
    id_ = id;
}

QString NvBaseItem::name() const
{
	return name_;
}

void NvBaseItem::setName( const QString &v )
{
	name_ = v;
}

QString NvBaseItem::description() const
{
	return description_;
}

void NvBaseItem::setDescription( const QString &v )
{
	description_ = v;
}

QString NvBaseItem::source() const
{
	return source_;
}

void NvBaseItem::setSource( const QString &v )
{
	source_ = v;
}

QImage NvBaseItem::icon() const
{
	return icon_;
}

void NvBaseItem::setIcon( const QImage &v )
{
	icon_ = v;
}

QDate NvBaseItem::date() const
{
	return date_;
}

void NvBaseItem::setDate( const QDate &v )
{
	date_ = v;
}


void NvBaseItem::setHasUpdate( bool v )
{
	updated_ = v;
}
