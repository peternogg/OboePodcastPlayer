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

signals:
    void positionChanged(qint64 position);

public slots:
    void playNext();
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
