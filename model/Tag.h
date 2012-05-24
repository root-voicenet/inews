#ifndef _TAG_H
#define _TAG_H

#include <QString>
#include <QHash>
#include <QDateTime>
#include <QList>
#include <QStringList>
#include <QStandardItemModel>
#include <QCoreApplication>

/**
 * Provides a model of all the tags that we met so far for the completer.
 */
class TagsTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    TagsTreeModel(QObject *parent = 0) : QStandardItemModel(parent) {}
    virtual ~TagsTreeModel() {}

    /* Lets change some flags to disable group box item in combobox. */
    Qt::ItemFlags flags(const QModelIndex & index) const;
};

class Entry;

/**
 * A tag that can be applied to entries, giving a way to group them
 * according to the user's preference.
 */
class Tag
{
    Q_DECLARE_TR_FUNCTIONS(Tag)
    friend class TagsManager;
private:
    quint32 _id;
    quint32 _parent;
    QString _name;

    const Tag &__createTag(QString tagName);

public:
    Tag(quint32 id, const QString &name, quint32 parent = 0) : _id(id), _name(name), _parent(parent) {}
    quint32 id() const { return _id; }
    quint32 parent() const { return _parent; }
    const QString &name() const { return _name; }

    /**
     * Returns whether this tag is valid.
     */
    bool isValid() const { return id() != 0; }

    bool operator==(const Tag &tag) const;
};

inline uint qHash(const Tag &tag)
{
    return tag.id();
}

class TagsManager
{
private:
    static Tag _invalid;
    /// A set of all the tags we know, used for auto-completion
    static TagsTreeModel knownTags;

public:
    static void init();
    static void cleanup();
    static TagsTreeModel *knownTagsModel() { return &knownTags; }

    /**
     * Returns true if the given string is valid for a tag.
     */
    static bool isValidTag(const QString &string);

    /**
     * Returns the tag corresponding to the string given as argument,
     * or the invalid tag if none is found.
     */
    static Tag getTag(const QString &tagString);

    static Tag getTag(quint32 id);


    /**
     * retrive tags for rss item
     */
    static QList<Tag> getRssTags(quint32 rss_id);
    static void setRssTags(quint32 rss_id, QList<Tag> tags);

    /**
     * retrive tags for node
     */
    static QList<Tag> getNodeTags(quint32 node_id);
    static void setNodeTags(quint32 node_id, QList<Tag> tags);
    /**
     * Returns the tag corresponding to the string given as argument,
     * creating it if none exists yet. The tag returned by this method
     * is always valid.
     */
    static Tag getOrCreateTag(const QString &tagString);

    static bool import(QVariant *resp);
};

#endif // _TAG_H

