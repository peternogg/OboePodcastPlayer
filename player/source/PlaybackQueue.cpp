#include "PlaybackQueue.h"

PlaybackQueue::PlaybackQueue(QObject *parent)
    : QAbstractTableModel(parent), _queue(), _player()
{
    connect(&_player, &QMediaPlayer::positionChanged, this, &PlaybackQueue::updateEpisodePlayback);
    connect(&_player, &QMediaPlayer::durationChanged, this, &PlaybackQueue::durationChanged);

    _player.setVolume(100);
    _player.setNotifyInterval(100);
}

void PlaybackQueue::prependEpisode(PodcastItem *item)
{
    beginInsertRows(QModelIndex(), 1, 1);

    if (_queue.empty()) {
        _queue.push_front(item);
    } else {
        // Put the new episode one spot behind the front
        _queue.insert(_queue.begin() + 1, item);
    }

    endInsertRows();
}

void PlaybackQueue::appendEpisode(PodcastItem *item) {
    auto const size = static_cast<int>(_queue.size());
    beginInsertRows(QModelIndex(), size, size);

    _queue.push_back(item);

    endInsertRows();
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
        beginRemoveRows(QModelIndex(), 0, 0);
        _queue.pop_front();
        endRemoveRows();

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
    if (_queue.empty())
        return;

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

void PlaybackQueue::addTime(qint64 dposition)
{
    auto newPosition = _player.position() + dposition;
    if (newPosition > _player.duration())
        playNext();
    else
        setPosition(newPosition);
}


int PlaybackQueue::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(_queue.size());
}

int PlaybackQueue::columnCount(const QModelIndex&) const
{
    // Cover art, title, length?
    return 3;
}

QVariant PlaybackQueue::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        auto* episode = _queue.at(static_cast<size_t>(index.row()));
        switch(index.column()) {
        case 0:
            return "Cover art!";
        case 1:
            return episode->title();
        case 2:
            return "Length goes here";
        }
    }

    return {};
}

QVariant PlaybackQueue::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical)
            return section + 1;

        switch(section) {
        case 0:
            return "Cover Art";
        case 1:
            return "Title";
        case 2:
            return "Length";
        }
    }

    return {};
}
