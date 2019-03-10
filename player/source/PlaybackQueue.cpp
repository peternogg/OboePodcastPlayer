#include "PlaybackQueue.h"

PlaybackQueue::PlaybackQueue(QObject *parent)
    : QObject(parent), _queue(), _player()
{
    connect(&_player, &QMediaPlayer::positionChanged, this, &PlaybackQueue::updateEpisodePlayback);
    _player.setVolume(100);
}

void PlaybackQueue::addEpisode(PodcastItem *item) {
    _queue.push_back(item);
}

bool PlaybackQueue::isPlaying() const {
    return _player.state() == QMediaPlayer::PlayingState;
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
            return;
        }
    }

    _player.setMedia(QUrl::fromLocalFile(_queue.front()->downloadPath()));
    _player.play();
}

void PlaybackQueue::updateEpisodePlayback(qint64 position) {
    _queue.front()->setLastTimestamp(position);
}

void PlaybackQueue::togglePlayback() {
    if (isPlaying())
        _player.pause();
    else
        _player.play();
}
