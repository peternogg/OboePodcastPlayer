#include "SubscriptionManager.h"

SubscriptionManager::SubscriptionManager()
    : QAbstractTableModel(), _parser(), _subscriptions()
{}

SubscriptionManager::~SubscriptionManager() {}

bool SubscriptionManager::subscribe_to(QString const& url) {
    auto feed = _parser.parse_url(url.toStdString());
    qDebug() << QString::fromStdString(feed.title) << ": " << QString::fromStdString(feed.pubDate);

    auto const rows = static_cast<int>(_subscriptions.size());
    beginInsertRows(QModelIndex(), rows, rows);
    _subscriptions.push_back(new Podcast(feed));
    endInsertRows();

    return true;
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
