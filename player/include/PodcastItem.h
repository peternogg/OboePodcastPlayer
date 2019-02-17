#ifndef PODCASTITEM_H
#define PODCASTITEM_H

#include <QDateTime>
#include <QObject>
#include <QUrl>

#include "Persistable.h"
#include "feedpp.h"

class PodcastItem : public Persistable
{
    Q_OBJECT
public:
    PodcastItem(QObject* parent = nullptr);
    PodcastItem(feedpp::item const& item, QObject *parent = nullptr);
    virtual ~PodcastItem() override;

    enum class ListenedState { Unheard, InProgress, Finished };
    enum class DownloadState { NotDownloaded, Downloading, Downloaded };

    Q_ENUM(ListenedState)
    Q_ENUM(DownloadState)

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QDateTime pubDate READ pubDate WRITE setPubDate)
    Q_PROPERTY(QString guid READ guid WRITE setGuid)
    Q_PROPERTY(QUrl enclosureUrl READ enclosureUrl WRITE setEnclosureUrl)
    Q_PROPERTY(QString enclosureType READ enclosureType WRITE setEnclosureType)
    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath)
    Q_PROPERTY(qint64 lastTimestamp READ lastTimestamp WRITE setLastTimestamp)
    Q_PROPERTY(ListenedState listenedState READ listenedState WRITE setListenedState)
    Q_PROPERTY(DownloadState downloadState READ downloadState WRITE setDownloadState)
    Q_PROPERTY(qint64 parentPodcast READ parentPodcast WRITE setParentPodcast)

    QString title() const;
    QString description() const;
    QDateTime pubDate() const;
    QString guid() const;
    QUrl enclosureUrl() const;
    QString enclosureType() const;
    QString downloadPath() const;
    qint64 lastTimestamp() const;
    ListenedState listenedState() const;
    DownloadState downloadState() const;
    qint64 parentPodcast() const;

    void setTitle(const QString& title);
    void setDescription(const QString& description);
    void setPubDate(const QDateTime& pubDate);
    void setGuid(const QString& guid);
    void setEnclosureUrl(const QUrl& enclosureUrl);
    void setEnclosureType(const QString& enclosureType);
    void setDownloadPath(const QString& downloadPath);
    void setLastTimestamp(const qint64& lastTimestamp);
    void setListenedState(const ListenedState& listenedState);
    void setDownloadState(const DownloadState& downloadState);
    void setParentPodcast(const qint64 &parentPodcast);

    virtual QString table() const override { return "PodcastItem"; }

private:
    QString _title;
    QString _description;
    QDateTime _pubDate;
    QString _guid;
    QUrl _enclosureUrl;
    QString _enclosureType;
    QString _downloadPath;
    qint64 _lastTimestamp;
    ListenedState _listenedState;
    DownloadState _downloadState;
    qint64 _parentPodcast;
};

#endif // PODCASTITEM_H
