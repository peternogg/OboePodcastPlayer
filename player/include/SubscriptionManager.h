#ifndef SUBSCRIPTIONMANAGER_H
#define SUBSCRIPTIONMANAGER_H

#include <QDebug>
#include <QString>
#include <QObject>
#include <QAbstractTableModel>
#include <QMetaObject>
#include <QFont>
#include <QtConcurrent>

#include "feedpp.h"
#include "parser.h"
#include "Podcast.h"
#include "Repository.h"

class SubscriptionManager : public QAbstractTableModel
{
    Q_OBJECT
public:
    SubscriptionManager(Repository& repo);
    virtual ~SubscriptionManager() override;

    bool subscribeTo(QString const& url);
    bool loadSubscriptions();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    std::vector<Podcast*> const& subscriptions() const { return _subscriptions; }

signals:
    void newSubscriptionAdded();

    void startedUpdateSubscriptions();
    void finishedUpdateSubscriptions();

public slots:
    void checkForUpdates();

private slots:
    void updateFinished();

private:
    feedpp::parser _parser;
    std::vector<Podcast*> _subscriptions;
    QFutureWatcher<void> _updateWatcher;
    Repository& _repo;

    bool storePodcast(Podcast* podcast) const;
    void doUpdate();
};

#endif // SUBSCRIPTIONMANAGER_H
