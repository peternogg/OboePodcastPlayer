QT += testlib core

# Keep GCC quiet about some stuff
QMAKE_CXXFLAGS += -Wno-pragmas -Wno-unused-variable

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    test_basicfeedpptests.cpp \
    ../player/feedpp/atom_parser.cpp \
    ../player/feedpp/date.cpp \
    ../player/feedpp/exception.cpp \
    ../player/feedpp/parser.cpp \
    ../player/feedpp/parser_factory.cpp \
    ../player/feedpp/rss_09x_parser.cpp \
    ../player/feedpp/rss_10_parser.cpp \
    ../player/feedpp/rss_parser.cpp \
    ../player/feedpp/utils.cpp

HEADERS += \
    ../player/feedpp/date.h \
    ../player/feedpp/exception.h \
    ../player/feedpp/feedpp.h \
    ../player/feedpp/log.h \
    ../player/feedpp/parser.h \
    ../player/feedpp/rsspp.h \
    ../player/feedpp/rsspp_internal.h \
    ../player/feedpp/types.h \
    ../player/feedpp/utils.h

INCLUDEPATH += ../player/include/ ../player/feedpp
unix:!macx: LIBS += -lxml2 -lcurl
