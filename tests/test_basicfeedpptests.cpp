#include <QtTest>

#include "parser.h"

class BasicFeedppTests : public QObject
{
    Q_OBJECT
public:
    BasicFeedppTests() {}
    virtual ~BasicFeedppTests() {}

private slots:
    void Feedpp_downloads_feeds() {
        feedpp::parser parser;
        // Download the Up First feed
        feedpp::feed feed = parser.parse_url("https://www.npr.org/rss/podcast.php?id=510318");

        QCOMPARE(feed.title, "Up First");
        QCOMPARE(feed.link, "http://www.npr.org/programs/morning-edition/");
        QVERIFY(feed.description.find("<em>") != std::string::npos);
    }
};

QTEST_APPLESS_MAIN(BasicFeedppTests)
#include "test_basicfeedpptests.moc"
