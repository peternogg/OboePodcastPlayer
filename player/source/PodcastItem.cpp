#include "PodcastItem.h"

PodcastItem::PodcastItem(feedpp::item const& data, QObject *parent)
    : QObject(parent),
      _title{QString::fromStdString(data.title)},
      _description{QString::fromStdString(data.description)},
      _pubDate{QDateTime::fromString(QString::fromStdString(data.pubDate))},
      _guid{QString::fromStdString(data.guid)},
      _enclosureUrl{QString::fromStdString(data.enclosure_url)},
      _enclosureType{QString::fromStdString(data.enclosure_type)}
{

}

PodcastItem::~PodcastItem() {}

QString PodcastItem::title() const
{
    return _title;
}

QString PodcastItem::description() const
{
    return _description;
}

QDateTime PodcastItem::pubDate() const
{
    return _pubDate;
}

QString PodcastItem::guid() const
{
    return _guid;
}

QUrl PodcastItem::enclosureUrl() const
{
    return _enclosureUrl;
}

QString PodcastItem::enclosureType() const
{
    return _enclosureType;
}
