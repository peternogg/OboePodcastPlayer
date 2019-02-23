#include "Podcast.h"

#include <QDebug>

Podcast::Podcast(QObject* parent)
    : Persistable(parent),
      _title{},
      _link{},
      _description{},
      _lastUpdate{},
      _updateLink{},
      _items{}
{}

Podcast::Podcast(feedpp::feed const& data, QObject* parent)
    : Persistable(parent),
      _title{QString::fromStdString(data.title)},
      _link{QString::fromStdString(data.link)},
      _description{QString::fromStdString(data.description)},
      _lastUpdate{},
      _updateLink{},
      _items{}
{
    // Map data.items => PodcastItems
    _items.resize(data.items.size());
    std::transform(data.items.begin(), data.items.end(), _items.begin(),
                   [this](auto item) { return new PodcastItem(item, this); });
}

Podcast::~Podcast(){}

bool Podcast::isDifferentFrom(Podcast const& other) const {
    // Two podcasts differ if any of their fields or episodes differ
    bool same = other.title() == _title;
    same &= other.link() == _link;
    same &= other.description() == _description;
    // If the number of episodes is different, the two must differ
    same &= other.items().size() == _items.size();

    for (size_t index = 0; same && index < _items.size(); index++) {
        same &= other.items()[index]->isDifferentFrom(_items[index]);
    }

    return !same;
}

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

std::vector<PodcastItem*> const& Podcast::items() const {
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

void Podcast::setItems(std::vector<PodcastItem*> const& items) {
    _items = items;
}

QUrl Podcast::updateLink() const
{
    return _updateLink;
}

void Podcast::setUpdateLink(const QUrl& updateLink)
{
    _updateLink = updateLink;
}
