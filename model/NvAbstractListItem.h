#ifndef NVABSTRACTLISTITEM_H
#define NVABSTRACTLISTITEM_H

#include <QString>
#include <QIcon>
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
    virtual QIcon icon() const = 0;
    virtual quint32 id() const = 0;


    virtual void setId(quint32 id) = 0;
    virtual void setIcon(const QIcon& icon) = 0;

    virtual Qt::ItemFlags flags() const { return Qt::NoItemFlags;}

signals:
	void needUpdate();

};
typedef QPointer< NvAbstractListItem > NvAbstractListItemPtr;

Q_DECLARE_INTERFACE(NvAbstractListItem, "inews.NvAbstractListItem")
#endif // NVABSTRACTLISTITEM_H
