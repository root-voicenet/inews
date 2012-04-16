#-------------------------------------------------
#
# Project created by QtCreator 2012-04-09T13:51:31
#
#-------------------------------------------------

QT       += core gui

TARGET = inews
TEMPLATE = app
INCLUDEPATH += "qxmlrpc"
DEFINES += "DEBUG=1"

SOURCES += main.cpp\
        mainwindow.cpp \
    qxmlrpc/xmlrpc/variant.cpp \
    qxmlrpc/xmlrpc/serverintrospection.cpp \
    qxmlrpc/xmlrpc/server.cpp \
    qxmlrpc/xmlrpc/response.cpp \
    qxmlrpc/xmlrpc/request.cpp \
    qxmlrpc/xmlrpc/client.cpp \
    connector.cpp \
    newsapplication.cpp \
    resourcemanager.cpp \
    feedmodel.cpp \
    textedit.cpp \
    node.cpp \
    file.cpp \
    rssitem.cpp \
    taxonomyterm.cpp \
    centerlawidget.cpp \
    rssviewwidget.cpp \
    nodeeditorwidget.cpp \
    taxonomywidget.cpp \
    rsslistitemdelegate.cpp \
    nvbaseobject.cpp \
    text/htmlexporter.cpp \
    text/cssparser.cpp \
    text/htmlparser.cpp \
    text/texthtmlimporter.cpp \
    model/NvSortFilterModel.cpp \
    model/NvRssItem.cpp \
    model/NvObjectModel.cpp \
    dbmanager.cpp

HEADERS  += mainwindow.h \
    qxmlrpc/xmlrpc/variant.h \
    qxmlrpc/xmlrpc/stable.h \
    qxmlrpc/xmlrpc/serverintrospection.h \
    qxmlrpc/xmlrpc/server_private.h \
    qxmlrpc/xmlrpc/server.h \
    qxmlrpc/xmlrpc/response.h \
    qxmlrpc/xmlrpc/request.h \
    qxmlrpc/xmlrpc/client.h \
    connector.h \
    newsapplication.h \
    resourcemanager.h \
    feedmodel.h \
    textedit.h \
    node.h \
    file.h \
    rssitem.h \
    taxonomyterm.h \
    centerlawidget.h \
    rssviewwidget.h \
    nodeeditorwidget.h \
    taxonomywidget.h \
    rsslistitemdelegate.h \
    nvbaseobject.h \
    text/htmlexporter.h \
    text/cssparser.h \
    text/textformat.h \
    text/texthtmlimporter.h \
    text/htmlparser.h \
    model/NvSortFilterModel.h \
    model/NvAbstractListItem.h \
    model/NvRssItem.h \
    model/NvObjectModel.h \
    dbmanager.h

FORMS    +=
QT += core gui network xml webkit sql

RESOURCES += \
    resource.qrc
