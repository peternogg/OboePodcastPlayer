#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSqlDatabase>

#include "Podcast.h"
#include "PodcastItem.h"
#include "Repository.h"

using namespace testing;

class RepositoryTests : public Test {
protected:
    QSqlDatabase db;

    RepositoryTests() : Test()
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("test.db");
    }

    void SetUp() override {
        ASSERT_TRUE(db.open());

        clearTables(db);
    }

    virtual ~RepositoryTests() override {
        db.close();
        db.removeDatabase("QSQLITE");
    }

private:
    void clearTables(QSqlDatabase& db) {
        char const* const table_names[] = { "Podcast", "PodcastItem", };

        for (auto name : table_names) {
            QSqlQuery query(QString("delete from %1;").arg(name), db);
            if (!query.exec())
                qDebug() << query.lastError();
        }
    }
};

TEST_F(RepositoryTests, RepositoryPersistsAnEpisode)
{
    PodcastItem* episode = new PodcastItem();
    Repository repo;
    auto const guid = "A test guid";
    auto const title = "A test title";
    auto const desc = "A test description";

    episode->setGuid(guid);
    episode->setTitle(title);
    episode->setDescription(desc);

    ASSERT_TRUE(repo.store(episode));
    auto const id = episode->id();

    delete episode;

    PodcastItem* fetchedEpisode = repo.fetch<PodcastItem>(id);

    EXPECT_EQ(fetchedEpisode->guid(), guid);
    EXPECT_EQ(fetchedEpisode->title(), title);
    EXPECT_EQ(fetchedEpisode->description(), desc);
}

TEST_F(RepositoryTests, RepositoryPersistsAPodcast)
{
    Podcast* podcast = new Podcast();
    Repository repo;

    auto const title = "A test podcast";
    auto const desc = "some text";
    auto const link = "http://example.com";

    podcast->setTitle(title);
    podcast->setDescription(desc);
    podcast->setLink(QUrl(link));
    podcast->setLastUpdate(QDateTime::currentDateTime());

    repo.store(podcast);
    auto const id = podcast->id();

    delete podcast;

    Podcast* fetchedPodcast = repo.fetch<Podcast>(id);

    EXPECT_EQ(fetchedPodcast->title(), title);
    EXPECT_EQ(fetchedPodcast->description(), desc);
    EXPECT_EQ(fetchedPodcast->link(), QUrl(link));
}
