#include "PodcastItem.h"

PodcastItem::PodcastItem(QObject* parent)
    : Persistable(parent),
      _title{},
      _description{},
      _pubDate{},
      _guid{},
      _enclosureUrl{},
      _enclosureType{},
      _downloadPath{},
      _parentPodcast{0}
{ }

PodcastItem::PodcastItem(feedpp::item const& data, QObject *parent)
    : Persistable(parent),
      _title{QString::fromStdString(data.title)},
      _description{QString::fromStdString(data.description)},
      _pubDate{QDateTime::fromString(QString::fromStdString(data.pubDate))},
      _guid{QString::fromStdString(data.guid)},
      _enclosureUrl{QString::fromStdString(data.enclosure_url)},
      _enclosureType{QString::fromStdString(data.enclosure_type)},
      _downloadPath{},
      _parentPodcast{0}
{ }

PodcastItem::~PodcastItem() {}

bool PodcastItem::isDifferentFrom(PodcastItem const& other) {
    return other.guid() != _guid; // Is this sufficient?
}

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

void PodcastItem::setTitle(const QString& title)
{
    _title = title;
}

void PodcastItem::setDescription(const QString& description)
{
    _description = description;
}

void PodcastItem::setPubDate(const QDateTime& pubDate)
{
    _pubDate = pubDate;
}

void PodcastItem::setGuid(const QString& guid)
{
    _guid = guid;
}

void PodcastItem::setEnclosureUrl(const QUrl& enclosureUrl)
{
    _enclosureUrl = enclosureUrl;
}

void PodcastItem::setEnclosureType(const QString& enclosureType)
{
    _enclosureType = enclosureType;
}

QString PodcastItem::downloadPath() const
{
    return _downloadPath;
}

void PodcastItem::setDownloadPath(const QString& downloadPath)
{
    _downloadPath = downloadPath;
}

qint64 PodcastItem::lastTimestamp() const
{
    return _lastTimestamp;
}

void PodcastItem::setLastTimestamp(const qint64& lastTimestamp)
{
    _lastTimestamp = lastTimestamp;
}

PodcastItem::ListenedState PodcastItem::listenedState() const
{
    if (_lastTimestamp == 0)
        return Unheard;
    else if (_lastTimestamp == -1)
        return Finished;
    else
        return InProgress;
}

PodcastItem::DownloadState PodcastItem::downloadState() const
{
    return _downloadPath.isEmpty() ? NotDownloaded : Downloaded;
}

qint64 PodcastItem::parentPodcast() const
{
    return _parentPodcast;
}

void PodcastItem::setParentPodcast(const qint64 &parentPodcast)
{
    _parentPodcast = parentPodcast;
}
