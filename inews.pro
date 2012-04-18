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
    resourcemanager.cpp \
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
    model/NvObjectModel.cpp \
    dbmanager.cpp \
    model/NvBaseItem.cpp \
    model/NvBaseItemDelegate.cpp \
    view/NvBaseListView.cpp \
    model/nvrssitem.cpp \
    model/NvRssCachedModel.cpp \
    loaders.cpp \
    model/NvLineItemDelegate.cpp

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
    resourcemanager.h \
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
    model/NvObjectModel.h \
    dbmanager.h \
    model/NvBaseItem.h \
    model/NvBaseItemDelegate.h \
    view/NvBaseListView.h \
    model/nvrssitem.h \
    model/NvRssCachedModel.h \
    model/NvObjectImporter.h \
    loaders.h \
    model/NvLineItemDelegate.h

FORMS    += \
    mainwindow.ui
QT += core gui network xml webkit sql

RESOURCES += \
    resource.qrc
