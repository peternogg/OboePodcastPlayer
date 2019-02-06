#include <QtTest>

#include "Podcast.h"
#include "PodcastItem.h"

#include "parser.h"

class BasicFeedppTests : public QObject
{
    Q_OBJECT
public:
    BasicFeedppTests() {}
    virtual ~BasicFeedppTests() {}

    feedpp::feed loadFeed(std::string const& filename) {
        feedpp::parser parser;
        auto feed = parser.parse_file(filename);

        return feed;
    }

private slots:
    void Feedpp_downloads_feeds() {
        feedpp::parser parser;
        // Download the Up First feed
        feedpp::feed feed = parser.parse_url("https://www.npr.org/rss/podcast.php?id=510318");

        QCOMPARE(feed.title, "Up First");
        QCOMPARE(feed.link, "http://www.npr.org/programs/morning-edition/");
        QVERIFY(feed.description.find("<em>") != std::string::npos);
    }

    /// Podcast can be generated from a feedpp::feed
    void Podcast_generates_from_feed() {
        auto feed = loadFeed("./data/UpFirst.rss");
        auto* podcast = new Podcast(feed);

        QCOMPARE(podcast->title().toStdString(), feed.title);
        QCOMPARE(podcast->description().toStdString(), feed.description);
        QCOMPARE(podcast->link().toString().toStdString(), feed.link);
    }

    void PodcastItem_generates_from_feed_item() {

    }

    void Podcast_maps_item_to_PodcastItem() {
        auto feed = loadFeed("./data/UpFirst.rss");
        auto* podcast = new Podcast(feed);

        QCOMPARE(feed.items.size(), podcast->items().size());
        for (size_t index = 0; index < feed.items.size(); index++) {
            auto item = feed.items[index];
            auto* podcastItem = podcast->items()[index];

            QCOMPARE(podcastItem->title().toStdString(), item.title);
            QCOMPARE(podcastItem->description().toStdString(), item.description);
            QCOMPARE(podcastItem->guid().toStdString(), item.guid);
            QCOMPARE(podcastItem->enclosureUrl().toString().toStdString(), item.enclosure_url);
        }
    }
};

QTEST_APPLESS_MAIN(BasicFeedppTests)
#include "test_basicfeedpptests.moc"
