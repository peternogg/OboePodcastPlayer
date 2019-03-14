#ifndef SUBSCRIPTIONMANAGER_H
#define SUBSCRIPTIONMANAGER_H

#include <QDebug>
#include <QString>
#include <QObject>
#include <QAbstractTableModel>
#include <QMetaObject>
#include <QFont>

#include "feedpp.h"
#include "parser.h"
#include "Podcast.h"
#include "Repository.h"
#include "DownloadManager.h"
#include "EpisodeModel.h"

class SubscriptionManager : public QAbstractTableModel
{
    Q_OBJECT
public:
    SubscriptionManager(Repository& repo, DownloadManager& downloadManager, QObject* parent = nullptr);
    virtual ~SubscriptionManager() override;

    /// Add a new subscription
    bool subscribeTo(QString const& url);
    /// Unsubscribe from some podcast and remove it from the database
    bool unsubscribeFrom(QModelIndex const& index);

    /// Fetch the user's current subscriptions from disk
    bool loadSubscriptions();
    /// Store the user's current subscriptions to disk
    bool storeSubscriptions();

    QAbstractTableModel* episodesFor(QModelIndex const& index);

    /// Mark all episodes of a podcast as heard
    void markAsHeard(QModelIndex const& index);
    /// Mark all episodes of a podcast as unheard (new)
    void markAsUnheard(QModelIndex const& index);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    std::vector<Podcast*> const& subscriptions() const { return _subscriptions; }

signals:
    void new_subscription_added();

public slots:
    void checkForUpdates();

private:
    feedpp::parser _parser;
    std::vector<Podcast*> _subscriptions;
    Repository& _repo;
    DownloadManager& _downloadManager;

    bool storePodcast(Podcast* podcast) const;
};

#endif // SUBSCRIPTIONMANAGER_H
