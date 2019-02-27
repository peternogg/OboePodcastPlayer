#-------------------------------------------------
#
# Project created by QtCreator 2019-01-16T14:20:12
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OboePodcastPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

INCLUDEPATH += ./include ./feedpp

# Keep GCC quiet about some stuff
QMAKE_CXXFLAGS += -Wno-pragmas -Wno-unused-variable

SOURCES += source/main.cpp \
           source/OboeWindow.cpp \
# Feedpp files
    feedpp/atom_parser.cpp \
    feedpp/date.cpp \
    feedpp/exception.cpp \
    feedpp/parser.cpp \
    feedpp/parser_factory.cpp \
    feedpp/rss_09x_parser.cpp \
    feedpp/rss_10_parser.cpp \
    feedpp/rss_parser.cpp \
    feedpp/utils.cpp \
    source/SubscriptionManager.cpp \
    source/Podcast.cpp \
    source/PodcastItem.cpp \
    source/Repository.cpp \
    source/DownloadManager.cpp \
    source/ItemDownload.cpp

HEADERS += include/OboeWindow.h \
# Feedpp files
    feedpp/date.h \
    feedpp/exception.h \
    feedpp/feedpp.h \
    feedpp/log.h \
    feedpp/parser.h \
    feedpp/rsspp.h \
    feedpp/rsspp_internal.h \
    feedpp/types.h \
    feedpp/utils.h \
    include/SubscriptionManager.h \
    include/Podcast.h \
    include/PodcastItem.h \
    include/Persistable.h \
    include/Repository.h \
    include/DownloadManager.h \
    include/ItemDownload.h

FORMS += \
        OboeWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lxml2 -lcurl

RESOURCES += \
    resources.qrc
