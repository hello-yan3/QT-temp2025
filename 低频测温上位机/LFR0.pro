QT       += core gui
QT       += serialport
QT       += charts
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/mainwindow
include ($$PWD/mainwindow/mainwindow.pri)

INCLUDEPATH += $$PWD/uiuser
include ($$PWD/uiuser/uiuser.pri)

INCLUDEPATH += $$PWD/page
include ($$PWD/page/page.pri)

INCLUDEPATH += $$PWD/datatool
include ($$PWD/datatool/datatool.pri)

SOURCES += \
    VersionRecord.cpp \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img/imgs.qrc \
    qss/qss.qrc

DISTFILES +=

HEADERS +=

RC_ICONS = logo.ico
