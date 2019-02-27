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

class SubscriptionManager : public QAbstractTableModel
{
    Q_OBJECT
public:
    SubscriptionManager(Repository& repo);
    virtual ~SubscriptionManager() override;

    bool subscribeTo(QString const& url);
    bool loadSubscriptions();

    void download(PodcastItem* item);

    QAbstractTableModel* episodesFor(QModelIndex const& index) const;

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
    DownloadManager _downloadManager;

    bool storePodcast(Podcast* podcast) const;
};

class EpisodeModel : public QAbstractTableModel {
    Q_OBJECT
public:
    EpisodeModel(Podcast* podcast);
    virtual ~EpisodeModel() override;

    PodcastItem* episodeFor(QModelIndex const& index);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
private:
    Podcast* _source;
};

#endif // SUBSCRIPTIONMANAGER_H
