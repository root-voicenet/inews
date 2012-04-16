#ifndef NVRSSITEM_H
#define NVRSSITEM_H

#include "NvAbstractListItem.h"
#include <QString>
#include <QImage>

class NvRssItem : public NvAbstractListItem
{
    QString name_, description_, source_;
	QImage icon_;
    quint32 id_;
	QDate date_;
protected:
	bool updated_;
public:
    NvRssItem(quint32 id, const QString & name = QString(), const QString & desc = QString());
    virtual ~NvRssItem(){}

	QString name() const;
	void setName(const QString &v);

	QString description() const;
	void setDescription(const QString &v);

    quint32 id() const;
    void setId(quint32 id);

	QString source() const;
	void setSource(const QString &v);

	QImage icon() const;
	void setIcon(const QImage &v);

	QDate date() const;
	void setDate(const QDate &v);

	void setHasUpdate(bool v);

};
#endif // NVRSSITEM_H
