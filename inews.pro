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
    view/NvBaseListView.cpp \
    model/nvrssitem.cpp \
    model/NvRssCachedModel.cpp \
    loaders.cpp \
    model/NvFeedModel.cpp \
    model/NvAbstractTreeItem.cpp \
    model/NvFeedCategory.cpp \
    view/NvFeedsTreeView.cpp \
    model/NvFeedItem.cpp \
    requestbuilder.cpp \
    dialogs/addeditlink.cpp \
    mediawindow.cpp \
    widgetimage.cpp \
    addiconimagethread.cpp \
    windowmanager.cpp \
    model/NvRssItemDelegate.cpp \
    model/NvMediaItem.cpp \
    model/NvMediaModel.cpp \
    mediatitledialog.cpp \
    model/NvNodeMediaItem.cpp \
    model/NvRssModel.cpp \
    feededitdialog.cpp

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
    view/NvBaseListView.h \
    model/nvrssitem.h \
    model/NvRssCachedModel.h \
    model/NvObjectImporter.h \
    loaders.h \
    model/NvFeedModel.h \
    model/NvAbstractTreeItem.h \
    model/NvFeedCategory.h \
    view/NvFeedsTreeView.h \
    model/NvAbstractFeedItem.h \
    model/NvFeedItem.h \
    requestbuilder.h \
    dialogs/addeditlink.h \
    mediawindow.h \
    widgetimage.h \
    addiconimagethread.h \
    windowmanager.h \
    model/NvRssItemDelegate.h \
    model/NvMediaItem.h \
    model/NvMediaModel.h \
    mediatitledialog.h \
    model/NvNodeMediaItem.h \
    model/NvRssModel.h \
    feededitdialog.h

QT += core gui network xml webkit sql

RESOURCES += \
    resource.qrc

FORMS += \
    dialogs/linkdialog.ui \
    mediawindow.ui \
    mediatitledialog.ui \
    feededitdialog.ui
