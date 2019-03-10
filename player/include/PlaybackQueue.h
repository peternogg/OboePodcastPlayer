#ifndef PLAYBACKQUEUE_H
#define PLAYBACKQUEUE_H

#include <deque>
#include <QObject>
#include <QMediaPlayer>

#include "PodcastItem.h"

class PlaybackQueue : public QObject
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

private slots:
    void updateEpisodePlayback(qint64 position);

private:
    // The queue of episodes to play
    // The currently playing item is at _queue.front()
    std::deque<PodcastItem*> _queue;
    QMediaPlayer _player;
};

#endif // PLAYBACKQUEUE_H
