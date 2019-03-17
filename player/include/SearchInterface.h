#ifndef SEARCHINTERFACE_H
#define SEARCHINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include <QUrlQuery>

#include <vector>

class SearchResult {
public:
    SearchResult(QString title, QString url, QString description)
        : title(title), url(url), description(description)
    {}
    SearchResult() {}

    QString getTitle() const;
    QString getUrl() const;
    QString getDescription() const;

private:
    QString title;
    QString url;
    QString description;
};

class SearchInterface : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SearchInterface(QString apiKey, QNetworkAccessManager* nwManager, QObject *parent = nullptr);

    bool searchInProgress() const;
    bool hasResults() const;
    std::vector<SearchResult> const& results() const;

signals:
    void searchFinished();

public slots:
    bool beginSearch(QString title);

private slots:
    void downloadFinished();

private:
    static QString const BaseURL;
    static QString const BasePath;
    std::vector<SearchResult> _results;
    QString _apiKey;
    QString _lastSearchString;
    QNetworkAccessManager* _nwManager;
    bool _searchInProgress;
    bool _hasResults;
    QNetworkReply* _currentReply;

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
};

#endif // SEARCHINTERFACE_H
