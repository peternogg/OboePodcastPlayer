#include "Podcast.h"

#include <QDebug>

Podcast::Podcast(QObject* parent)
    : Persistable(parent),
      _title{},
      _link{},
      _description{},
      _lastUpdate{},
      _items{}
{}

Podcast::Podcast(feedpp::feed const& data, QObject* parent)
    : Persistable(parent),
      _title{QString::fromStdString(data.title)},
      _link{QString::fromStdString(data.link)},
      _description{QString::fromStdString(data.description)},
      _lastUpdate{},
      _items{}
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

void Podcast::setTitle(const QString &title)
{
    _title = title;
}

void Podcast::setLink(const QUrl &link)
{
    _link = link;
}

void Podcast::setDescription(const QString &description)
{
    _description = description;
}

void Podcast::setLastUpdate(const QDateTime &lastUpdate)
{
    _lastUpdate = lastUpdate;
}
