GOOGLETEST_DIR = ../../googletest

include(gtest_dependency.pri)

QT += sql
TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread

# Keep GCC quiet about some stuff
QMAKE_CXXFLAGS += -Wno-pragmas -Wno-unused-variable

INCLUDEPATH += ../player/include/ ../player/feedpp/

HEADERS += \
        tst_repositorytests.h \
    ../player/include/Repository.h \
    ../player/include/PodcastItem.h \
    ../player/include/Podcast.h \
    ../player/include/Persistable.h \
    ../player/feedpp/utils.h \
    ../player/feedpp/types.h \
    ../player/feedpp/rsspp_internal.h \
    ../player/feedpp/rsspp.h \
    ../player/feedpp/parser.h \
    ../player/feedpp/log.h \
    ../player/feedpp/feedpp.h \
    ../player/feedpp/exception.h \
    ../player/feedpp/date.h \
    testpersistable.h

SOURCES += \
        main.cpp \
    ../player/source/Repository.cpp \
    ../player/source/PodcastItem.cpp \
    ../player/source/Podcast.cpp \
    ../player/feedpp/utils.cpp \
    ../player/feedpp/rss_parser.cpp \
    ../player/feedpp/rss_10_parser.cpp \
    ../player/feedpp/rss_09x_parser.cpp \
    ../player/feedpp/parser_factory.cpp \
    ../player/feedpp/parser.cpp \
    ../player/feedpp/exception.cpp \
    ../player/feedpp/date.cpp \
    ../player/feedpp/atom_parser.cpp

unix:!macx: LIBS += -lxml2 -lcurl
