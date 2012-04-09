#ifndef FILE_H
#define FILE_H

#include <QString>

class File  {

public:
    File(const QString &url, const QString description = "");

    QString getUrl() const { return m_url; }
    QString getDescription() const { return m_description; }

    bool operator == (const File& other) const;
private:
    QString m_url;
    QString m_description;
};


#endif // FILE_H
