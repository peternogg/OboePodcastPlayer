#include "SubscriptionManager.h"

SubscriptionManager::SubscriptionManager(Repository& repo)
    : QAbstractTableModel(),
      _parser(),
      _subscriptions(),
      _updateWatcher(),
      _repo(repo)
{
    connect(&_updateWatcher, &QFutureWatcher<std::vector<Podcast*>*>::finished,
            this, &SubscriptionManager::updateFinished);
}

SubscriptionManager::~SubscriptionManager() {}

bool SubscriptionManager::subscribeTo(QString const& url) {
    feedpp::feed feed;

    try {
        feed = _parser.parse_url(url.toStdString());
    } catch (std::exception&) {
        qDebug() << "Exception when adding subscription";
        return false;
    }

    auto const rows = static_cast<int>(_subscriptions.size());

    beginInsertRows(QModelIndex(), rows, rows);

    _subscriptions.push_back(new Podcast(feed));
    _subscriptions.back()->setLastUpdate(QDateTime::currentDateTime());
    _subscriptions.back()->setUpdateLink(url);

    endInsertRows();

    storePodcast(_subscriptions.back());

    emit newSubscriptionAdded();

    return true;
}

bool SubscriptionManager::loadSubscriptions()
{
    _subscriptions = _repo.fetchAll<Podcast>();
    beginInsertRows(QModelIndex(), 0, static_cast<int>(_subscriptions.size()));
    endInsertRows();

    return true;
}

void SubscriptionManager::checkForUpdates() {
    _updateWatcher.setFuture(QtConcurrent::run(this, &SubscriptionManager::doUpdate));
    emit startedUpdateSubscriptions();
}

void SubscriptionManager::doUpdate() {
    QSqlDatabase db = QSqlDatabase::database("update");
    if (!db.isOpen()) {
        qDebug() << db.lastError();
    }

    for (size_t index = 0; index < _subscriptions.size(); index++) {
        auto* podcast = _subscriptions[index];
        auto* updatedPodcast = new Podcast(_parser.parse_url(
                                               podcast->updateLink().toString().toStdString()));

        if (podcast->isDifferentFrom(*updatedPodcast)) {
            updatedPodcast->setUpdateLink(podcast->updateLink());
            updatedPodcast->setId(podcast->id());
            updatedPodcast->setLastUpdate(QDateTime::currentDateTime());
            storePodcast(updatedPodcast);

            delete podcast;
            _subscriptions[index] = updatedPodcast;
        }
    }
}

void SubscriptionManager::updateFinished() {
    emit dataChanged(QModelIndex(), QModelIndex(), { Qt::DisplayRole, Qt::FontRole });
    emit finishedUpdateSubscriptions();
}

bool SubscriptionManager::storePodcast(Podcast* podcast) const {
    _repo.store(podcast);
    qDebug() << "storePodcast id = " << podcast->id();
    for (auto episode : podcast->items()) {
        episode->setParentPodcast(podcast->id());
        _repo.store(episode);
    }

    return true; // FIXME: does nothing
}

int SubscriptionManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(_subscriptions.size());
}

int SubscriptionManager::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // Cover art, title, description
    return 3;
}

QVariant SubscriptionManager::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        auto const* podcast = _subscriptions.at(static_cast<size_t>(index.row()));

        switch(index.column()) {
        case 0:
            return podcast->updateLink();
        case 1:
            return podcast->title();
        case 2:
            return podcast->description();
        default:
            return {};
        }
    } else if (role == Qt::FontRole) {
        if (index.row() % 2 == 1) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }

    return {};
}

QVariant SubscriptionManager::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical)
        return {};

    if (role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return "Cover Art";
        case 1:
            return "Title";
        case 2:
            return "Description";
        default:
            return {};
        }
    }

    return {};
}
