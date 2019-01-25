#include "RSSFeedParser.h"

IRSSFeedParser::IRSSFeedParser() {};
IRSSFeedParser::~IRSSFeedParser() {};

RSSFeedParser::RSSFeedParser()
    : _reader{}
{}

RSSFeedParser::~RSSFeedParser() {}

std::optional<RSSFeed> RSSFeedParser::parse(QString const& data) {
    RSSFeed feed;
    _reader.addData(data);

    /*
    <Doc>
        <rss ~~~> -> parse_rss()
    </Doc>
    */

    // Read start of document, start of RSS tag
    if (_reader.readNext() != QXmlStreamReader::StartDocument)
        return {};

    auto token = _reader.readNext();
    if (token != QXmlStreamReader::StartElement || _reader.name() != "rss")
        return {};

    if (!parse_rss(feed))
        return {};

    if (_reader.readNext() != QXmlStreamReader::EndDocument)
        return {};

    return feed;
}

bool RSSFeedParser::parse_rss(RSSFeed& feed) {
    qDebug() << "Saw the RSS tag";
    // Handle RSS stuff
    // channel (title | description) item*
    // in any order
    // channel = StartElement(channel) Text EndElement(channel)
    // title = StartElement(title) Text EndElement(title)
    // description = StartElement(description) Text EndElement(description)
    // item = StartElement(item) item_contents EndElement(item)
    // While (there are tokens left)
    // Read a token, classify it
    // Handle appropriately
    // Loop
    do {
    auto token = _reader.readNext();
    if (token != QXmlStreamReader::StartElement)
        return false;

    if (_reader.name() == "channel")
        parse_channel(feed);
    } while ()

    qDebug() << "Name: " << _reader.name();
    return true;
}

