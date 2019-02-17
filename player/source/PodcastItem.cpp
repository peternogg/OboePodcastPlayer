#include "PodcastItem.h"

PodcastItem::PodcastItem(QObject* parent)
    : QObject(parent),
      _title{},
      _description{},
      _pubDate{},
      _guid{},
      _enclosureUrl{},
      _enclosureType{},
      _downloadPath{},
      _listenedState{ListenedState::Unheard},
      _downloadState{DownloadState::NotDownloaded}
{ }

PodcastItem::PodcastItem(feedpp::item const& data, QObject *parent)
    : QObject(parent),
      _title{QString::fromStdString(data.title)},
      _description{QString::fromStdString(data.description)},
      _pubDate{QDateTime::fromString(QString::fromStdString(data.pubDate))},
      _guid{QString::fromStdString(data.guid)},
      _enclosureUrl{QString::fromStdString(data.enclosure_url)},
      _enclosureType{QString::fromStdString(data.enclosure_type)},
      _downloadPath{},
      _listenedState{ListenedState::Unheard},
      _downloadState{DownloadState::NotDownloaded}
{ }

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
    return _listenedState;
}

void PodcastItem::setListenedState(const ListenedState& listenedState)
{
    _listenedState = listenedState;
}

PodcastItem::DownloadState PodcastItem::downloadState() const
{
    return _downloadState;
}

void PodcastItem::setDownloadState(const DownloadState& downloadState)
{
    _downloadState = downloadState;
}
