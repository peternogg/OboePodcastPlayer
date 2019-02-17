#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSqlDatabase>

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
    }

    virtual ~RepositoryTests() override {
        db.close();
        db.removeDatabase("QSQLITE");
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
