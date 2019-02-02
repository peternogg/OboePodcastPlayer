QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_RSSFeedTests.cpp \
            ../player/source/RSSFeed.cpp \
            ../player/source/RSSItem.cpp \
            ../player/source/RSSFeedParser.cpp

HEADERS +=  ../player/include/RSSFeed.h \
            ../player/include/RSSItem.h \
            ../player/include/RSSFeedParser.h

INCLUDEPATH += ../player/
