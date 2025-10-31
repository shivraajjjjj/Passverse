QT       += core gui network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt gui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddialog.cpp \
    authdialog.cpp \
    creatempassdialog.cpp \
    cryptohelper.cpp \
    databasemanager.cpp \
    deletedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    sessionkey.cpp

HEADERS += \
    adddialog.h \
    authdialog.h \
    creatempassdialog.h \
    cryptohelper.h \
    databasemanager.h \
    deletedialog.h \
    mainwindow.h \
    picosha2.h \
    sessionkey.h

FORMS += \
    adddialog.ui \
    authdialog.ui \
    creatempassdialog.ui \
    deletedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
LIBS += -L"C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD" -llibssl -llibcrypto

DISTFILES +=
