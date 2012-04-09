#include "file.h"

File::File(const QString &url, const QString description) :
    m_url(url), m_description(description)
{

}


bool File::operator == (const File& other) const
{
    return m_url == other.m_url;
}
