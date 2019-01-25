#ifndef RSSFEEDPARSER_H
#define RSSFEEDPARSER_H

#include <iostream>
#include <optional>
#include <QDebug>
#include <QXmlStreamReader>

#include "RSSFeed.h"

class IRSSFeedParser {
public:
    IRSSFeedParser();
    virtual ~IRSSFeedParser();
    //virtual RSSFeed parse(QByteArray const& data) = 0;
    virtual RSSFeed* parse(QString const& data) = 0;
};

class RSSFeedParser : IRSSFeedParser
{
public:
    RSSFeedParser();
    virtual ~RSSFeedParser() override;

    virtual RSSFeed* parse(QString const& data) override;
    //virtual RSSFeed parse(QByteArray const& data) override;

private:
    QXmlStreamReader _reader;

    /// Load RSS channel information
    bool parse_rss(RSSFeed* feed);
    bool parse_channel(RSSFeed* feed);
    bool parse_item(RSSFeed* feed);

    /// Load a title for something which has a title
    template <typename T>
    bool parse_title(T* item);
    template <typename T>
    bool parse_description(T* item);
};

#endif // RSSFEEDPARSER_H
