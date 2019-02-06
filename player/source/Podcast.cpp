#include "Podcast.h"

#include <QDebug>

Podcast::Podcast(feedpp::feed const& data, QObject* parent)
    : QObject(parent),
      _title{QString::fromStdString(data.title)},
      _link{QString::fromStdString(data.link)},
      _description{QString::fromStdString(data.description)}
{
    // Map data.items => PodcastItems
    _items.resize(data.items.size());
    std::transform(data.items.begin(), data.items.end(), _items.begin(),
                   [this](auto item) { return new PodcastItem(item, this); });
}

Podcast::~Podcast(){}

QString Podcast::title() const
{
    return _title;
}

QUrl Podcast::link() const
{
    return _link;
}

QString Podcast::description() const
{
    return _description;
}

QDateTime Podcast::lastUpdate() const
{
    return _lastUpdate;
}

std::vector<PodcastItem*> Podcast::items() const {
    return _items;
}
