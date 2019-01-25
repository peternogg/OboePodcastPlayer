QT += testlib

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_RSSFeedTests.cpp \
            ../player/RSSFeed.cpp \
            ../player/RSSItem.cpp \
            ../player/RSSFeedParser.cpp

HEADERS +=  ../player/RSSFeed.h \
            ../player/RSSItem.h \
            ../player/RSSFeedParser.h

INCLUDEPATH += ../player/
