#ifndef PLAYBACKQUEUE_H
#define PLAYBACKQUEUE_H

#include <deque>
#include <QObject>
#include <QMediaPlayer>
#include <QAbstractTableModel>

#include "PodcastItem.h"
#include "util.h"

class PlaybackQueue : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PlaybackQueue(QObject *parent = nullptr);

    void addEpisode(PodcastItem* item);
    bool isPlaying() const;

    qint64 currentEpisodeLength() const;

signals:
    /// Emitted occasionally to update the position of the current episode
    void positionChanged(qint64 position);
    /// Emitted when the duration of the playing episode changes
    void durationChanged(qint64 duration);
    /// Emitted when the episode currently playing changes.
    /// item is null if the player reaches the end and stops.
    void episodeChanged(PodcastItem* item);

public slots:
    /// Skip forwards to the next track. Stops playing if the queue is empty
    void playNext();
    /// Pauses/plays the currently playing episode.
    void togglePlayback();
    /// Update the position in playback
    void setPosition(qint64 newPosition);

private slots:
    void updateEpisodePlayback(qint64 position);

private:
    // The queue of episodes to play
    // The currently playing item is at _queue.front()
    std::deque<PodcastItem*> _queue;
    QMediaPlayer _player;

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // PLAYBACKQUEUE_H
