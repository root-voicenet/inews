#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QList>

class File;
class Node
{
    friend class Connector;
public:
    Node(const QString& title, const QString& body);
    virtual ~Node();
    QString getTitle() const { return m_title; }
    QString &getBody() { return m_body; }

    void attachFile(File* file);
    void removeFile(File* file);
private:
    QString m_title;
    QString m_body;
    QList<File*> m_attached;

};

#endif // NODE_H
