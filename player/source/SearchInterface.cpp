#include "SearchInterface.h"

QString const SearchInterface::BaseURL = "https://api.listennotes.com/api/v1/search/";
QString const SearchInterface::BasePath = "";

SearchInterface::SearchInterface(QString apiKey, QNetworkAccessManager* nwManager, QObject *parent)
    : QAbstractTableModel(parent), _results(), _apiKey(apiKey), _lastSearchString(""), _nwManager(nwManager), _searchInProgress(false), _hasResults(false)
{
    connect(_nwManager, &QNetworkAccessManager::finished, this, &SearchInterface::downloadFinished);
}

bool SearchInterface::beginSearch(QString title) {
    if (_searchInProgress)
        return false;

    QUrl apiURL(BaseURL);
    QUrlQuery query;

    query.addQueryItem("q", title);
    query.addQueryItem("type", "podcast");

    apiURL.setQuery(query);


    qDebug() << "Request: " << apiURL.toString();

    QNetworkRequest request(apiURL);
    request.setRawHeader("X-RapidAPI-Key", _apiKey.toUtf8());

    _nwManager->get(request);

    _lastSearchString = title;
    _searchInProgress = true;

    return true;
}

bool SearchInterface::searchInProgress() const {
    return _searchInProgress;
}

bool SearchInterface::hasResults() const {
    return _hasResults;
}

std::vector<SearchResult> const& SearchInterface::results() const {
    return _results;
}

void SearchInterface::downloadFinished(QNetworkReply* reply) {
    reply->deleteLater();

    auto results = QJsonDocument::fromJson(reply->readAll());
    if (results.isNull()) {
        qDebug() << "Search result parse failed";
    }

    auto searchResults = results["results"].toArray();

    beginRemoveRows(QModelIndex(), 0, _results.size() - 1);
    _results.resize(searchResults.count());
    endRemoveRows();

    if (searchResults.isEmpty()) {
        _hasResults = false;

        beginInsertRows(QModelIndex(), 0, 1);
        _results.push_back(SearchResult("No results for that query", "", ""));
        endInsertRows();
    } else {
        _hasResults = true;

        beginInsertRows(QModelIndex(), 0, searchResults.count() - 1);
        std::transform(searchResults.begin(), searchResults.end(), _results.begin(), [](QJsonValue item) {
            return SearchResult(item["title_original"].toString(),
                                item["rss"].toString(),
                                item["description_original"].toString());
        });
        endInsertRows();
    }

    _searchInProgress = false;
    emit searchFinished();
}

QString SearchResult::getUrl() const
{
    return url;
}

QString SearchResult::getDescription() const
{
    return description;
}

QString SearchResult::getTitle() const
{
    return title;
}


int SearchInterface::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _results.size();
}

int SearchInterface::columnCount(const QModelIndex &parent) const
{
    return 2; // Title + description
}

QVariant SearchInterface::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        auto const item = _results.at(index.row());
        switch(index.column()) {
        case 0:
            return item.getTitle();
        case 1:
            return item.getDescription();
        }
    }

    return {};
}
