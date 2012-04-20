#ifndef NVABSTRACTLISTITEM_H
#define NVABSTRACTLISTITEM_H

#include <QString>
#include <QImage>
#include <QDateTime>
#include <QObject>
#include <QPointer>

class NvAbstractListItem: public QObject
{
	Q_OBJECT

public:	
    enum {
        hasUpdate = 0
    };

    virtual ~NvAbstractListItem(void){}
	virtual QString name() const = 0;
	virtual QString description() const = 0;

	virtual QString source() const = 0;
	virtual QImage icon() const = 0;
    virtual QDateTime date() const = 0;
    virtual quint32 id() const = 0;
    virtual bool updated() const = 0;
    virtual bool promoted() const = 0;

	virtual void setDescription(const QString&) = 0;
	virtual void setSource(const QString&)= 0;
    virtual void setDate(const QDateTime&) = 0;
    virtual void setId(quint32 id) = 0;
    virtual void setUpdated(bool) = 0;
    virtual void setPromoted(bool) = 0;

    virtual Qt::ItemFlags flags() const { return Qt::NoItemFlags;}

signals:
	void needUpdate();

};
typedef QPointer< NvAbstractListItem > NvAbstractListItemPtr;

Q_DECLARE_INTERFACE(NvAbstractListItem, "inews.NvAbstractListItem")
#endif // NVABSTRACTLISTITEM_H
