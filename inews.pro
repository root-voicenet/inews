#-------------------------------------------------
#
# Project created by QtCreator 2012-04-09T13:51:31
#
#-------------------------------------------------

QT       += core gui

TARGET = inews
TEMPLATE = app
INCLUDEPATH += "qxmlrpc"
DEFINES += "XMLRPC_DEBUG=1"
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
    taxonomymodel.cpp \
    newsapplication.cpp \
    resourcemanager.cpp \
    feedmodel.cpp \
    textedit.cpp \
    node.cpp \
    file.cpp \
    rssitem.cpp \
    taxonomyterm.cpp

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
    taxonomymodel.h \
    newsapplication.h \
    resourcemanager.h \
    feedmodel.h \
    textedit.h \
    node.h \
    file.h \
    rssitem.h \
    taxonomyterm.h

FORMS    += \
    mainwindow.ui
QT += core gui network xml
