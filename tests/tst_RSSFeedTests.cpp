#include <QtTest>

// add necessary includes here
#include "RSSFeed.h"
#include "RSSFeedParser.h"

class RSSFeedTests : public QObject
{
    Q_OBJECT

public:
    RSSFeedTests();
    ~RSSFeedTests();

private slots:
    void test_recognizes_RSS_feed();
//    void test_ignores_nonRSS_feed();
};

RSSFeedTests::RSSFeedTests() {}

RSSFeedTests::~RSSFeedTests() {}

void RSSFeedTests::test_recognizes_RSS_feed() {
    RSSFeedParser parser;
    QString const data =
            "<rss version=\"2.0\">"
                "<channel>"
                    "<title>Test Podcast</title>"
                    "<pubDate>Thu, 24 Jan 2019 18:31:51 +0000</pubDate>"
                    "<description>Description</description>"
                    "<link>https://example.com</link>"
                    "<item>"
                        "<title>An item</title>"
                        "<enclosure length=\"1212\" type=\"audio/mpeg\" url=\"https://example.com/test.mp3\"/>"
                    "</item"
                "</channel>"
            "</rss>"
            ;

    QVERIFY2(parser.parse(data) != nullptr, "Failed to parse RSS");
}

//void RSSFeedTests::test_ignores_nonRSS_feed() {

//}

QTEST_APPLESS_MAIN(RSSFeedTests)

#include "tst_RSSFeedTests.moc"
