#include "PlaybackQueue.h"

PlaybackQueue::PlaybackQueue(QObject *parent)
    : QObject(parent), _queue(), _player()
{
    connect(&_player, &QMediaPlayer::positionChanged, this, &PlaybackQueue::updateEpisodePlayback);
    connect(&_player, &QMediaPlayer::durationChanged, this, &PlaybackQueue::durationChanged);

    _player.setVolume(100);
    _player.setNotifyInterval(100);
}

void PlaybackQueue::addEpisode(PodcastItem *item) {
    _queue.push_back(item);
}

bool PlaybackQueue::isPlaying() const {
    return _player.state() == QMediaPlayer::PlayingState;
}

qint64 PlaybackQueue::currentEpisodeLength() const {
    return _player.duration();
}

void PlaybackQueue::playNext() {
    if (_queue.empty())
        return;

    if (isPlaying()) {
        // If the player was playing, and we were asked to skip over
        // the last thing in the queue, then just stop
        _queue.pop_front();
        if (_queue.empty()) {
            _player.stop();
            emit episodeChanged(nullptr);
            return;
        }
    }

    _player.setMedia(QUrl::fromLocalFile(_queue.front()->downloadPath()));
    _player.play();
    emit episodeChanged(_queue.front());
}

void PlaybackQueue::updateEpisodePlayback(qint64 position) {
    _queue.front()->setLastTimestamp(position);
    emit positionChanged(position);
}

void PlaybackQueue::togglePlayback() {
    if (isPlaying())
        _player.pause();
    else
        _player.play();
}

void PlaybackQueue::setPosition(qint64 newPosition) {
    _player.setPosition(newPosition);
}
