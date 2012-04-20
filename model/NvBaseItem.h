#ifndef NVBASEITEM_H
#define NVBASEITEM_H

#include "NvAbstractListItem.h"
#include <QString>
#include <QImage>

class NvBaseItem : public NvAbstractListItem
{
    QString name_, description_, source_;
    QImage icon_;
    quint32 id_;
    QDateTime date_;
protected:
	bool updated_;
    bool promoted_;
public:
    NvBaseItem(quint32 id, const QString & name = QString(), const QString & desc = QString());
    virtual ~NvBaseItem(){}

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

    QDateTime date() const;
    void setDate(const QDateTime &v);

    bool updated() const;
    void setUpdated(bool v);

    bool promoted() const;
    void setPromoted(bool v);

};
#endif // NVBASEITEM_H
