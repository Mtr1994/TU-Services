QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -execution-charset:utf-8

VERSION = 21.05.20.02

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:/spdlog/include

DESTDIR = ..\bin

SOURCES += \
    Dialog/dialogabout.cpp \
    Dialog/dialognote.cpp \
    Dialog/dialogtcpclientargs.cpp \
    Dialog/dialogtcpserverargs.cpp \
    Log/logger.cpp \
    Manager/clientmanager.cpp \
    Manager/servermanager.cpp \
    Net/tcpserver.cpp \
    Net/tcpsocket.cpp \
    Public/softconstants.cpp \
    Widget/widgettabcontent.cpp \
    Widget/widgettabpages.cpp \
    Widget/widgettreeview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Dialog/dialogabout.h \
    Dialog/dialognote.h \
    Dialog/dialogtcpclientargs.h \
    Dialog/dialogtcpserverargs.h \
    Log/logger.h \
    Manager/clientmanager.h \
    Manager/servermanager.h \
    Net/tcpserver.h \
    Net/tcpsocket.h \
    Public/defines.h \
    Public/softconstants.h \
    Public/threadpool.h \
    Public/treeitemdelegate.h \
    Widget/widgettabcontent.h \
    Widget/widgettabpages.h \
    Widget/widgettreeview.h \
    mainwindow.h

FORMS += \
    Dialog/dialogabout.ui \
    Dialog/dialognote.ui \
    Dialog/dialogtcpclientargs.ui \
    Dialog/dialogtcpserverargs.ui \
    Widget/widgettabcontent.ui \
    Widget/widgettabpages.ui \
    Widget/widgettreeview.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS = icon.ico

#0x0800代表和系统当前语言一致
RC_LANG = 0x0800
