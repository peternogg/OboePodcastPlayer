#include "EpisodeModel.h"

EpisodeModel::EpisodeModel(Podcast* source, Repository& repo, QObject* parent)
    : QAbstractTableModel(parent),
      _source(source),
      _repo(repo)
{}

EpisodeModel::~EpisodeModel()
{}

PodcastItem* EpisodeModel::episodeFor(const QModelIndex &index) {
    if (index.row() >= static_cast<int>(_source->items().size()))
        return nullptr;

    return _source->items()[static_cast<size_t>(index.row())];
}

void EpisodeModel::downloadStartedFor(const QModelIndex &, ItemDownload *) {
//    _repo.store(download->item());
//    connect(download, &ItemDownload::downloadFinished, this, &EpisodeModel::downloadFinished);

    //    emit dataChanged(index, createIndex(index.row() + 1, 3), { Qt::DisplayRole, Qt::FontRole });
}

bool EpisodeModel::deleteDownloadedEpisode(const QModelIndex& index) {
    auto* const episode = episodeFor(index);

    if (episode->downloadState() != PodcastItem::Downloaded)
        return false;

    QString file = episode->downloadPath();
    episode->setDownloadPath("");

    _repo.store(episode);

    emit dataChanged(this->index(index.row(), 2), this->index(index.row(), 2), { Qt::DisplayRole });

    return QFile(file).remove();
}

int EpisodeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(_source->items().size());
}

int EpisodeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // Title, Description, Length, Download State
    return 4;
}

QVariant EpisodeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        auto* item = _source->items().at(static_cast<size_t>(index.row()));
        switch(index.column()) {
        case 0:
            return item->title();
        case 1:
            return item->enclosureUrl();
        case 2:
            return static_cast<int>(item->downloadState());
        case 3:
            return item->description();
        default:
            return {};
        }
    } else if (role == Qt::FontRole) {
        auto* item = _source->items().at(static_cast<size_t>(index.row()));
        QFont font;

        switch(item->listenedState()) {
        case PodcastItem::Unheard:
            font.setBold(true);
        break;
        case PodcastItem::InProgress:
            font.setItalic(true);
        break;
        default: // Do nothing to the font
        break;
        }

        return font;
    }

    return {};
}


QVariant EpisodeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical)
        return {};

    if (role != Qt::DisplayRole)
        return {};

    switch (section) {
    case 0:
        return "Title";
    case 1:
        return "Download URL";
    case 2:
        return "Downloaded?";
    case 3:
        return "Description";
    default:
        return {};
    }
}

void EpisodeModel::downloadFinished(PodcastItem *item) {
    _repo.store(item);

//    auto iter = std::find(_source->items().begin(), _source->items().end(), item);

//    if (iter == _source->items().end()) {
//        qDebug() << "downloadFinished:: find failed";
//        return;
//    }

//    auto topLeft = createIndex(static_cast<int>(iter - _source->items().begin()), 3);
//    auto bottomRight = createIndex(static_cast<int>(iter - _source->items().begin() + 1), 3);

    //    emit dataChanged(topLeft, bottomRight, { Qt::DisplayRole, Qt::FontRole });
}

void EpisodeModel::markUnheard(const QModelIndex& index) {
    episodeFor(index)->setLastTimestamp(0);
    dataChanged(index, index, { Qt::FontRole, Qt::DisplayRole });
}

void EpisodeModel::markListenedTo(const QModelIndex& index) {
    episodeFor(index)->setLastTimestamp(-1);
    dataChanged(index, index, { Qt::FontRole, Qt::DisplayRole });
}
