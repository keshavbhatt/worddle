#-------------------------------------------------
#
# Project created by QtCreator 2022-02-07T23:46:31
#
#-------------------------------------------------

QT       += core gui network webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = worddle
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# No debug output in release mode
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT


# Set program version
VERSION = 1.0
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

include(widgets/Spinner/Spinner.pri)
include(widgets/WebEnginePlayer/WebEnginePlayer.pri)
include(widgets/SlidingStackedWidget/SlidingStackedWidget.pri)
include(widgets/RateApp/RateApp.pri)
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        rungaurd.cpp

HEADERS += \
        mainwindow.h \
        rungaurd.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    icons.qrc

# Default rules for deployment.
isEmpty(PREFIX){
 PREFIX = /usr
}

BINDIR  = $$PREFIX/bin
DATADIR = $$PREFIX/share

target.path = $$BINDIR

icon.files = icons/worddle.png
icon.path = $$DATADIR/icons/hicolor/512x512/apps/

desktop.files = worddle.desktop
desktop.path = $$DATADIR/applications/

INSTALLS += target icon desktop


