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

class SubscriptionManager : public QAbstractTableModel
{
    Q_OBJECT
public:
    SubscriptionManager();
    virtual ~SubscriptionManager() override;

    bool subscribe_to(QString const& url);
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    std::vector<Podcast*> const& subscriptions() const { return _subscriptions; }

signals:
    void new_subscription_added();

private:
    feedpp::parser _parser;
    std::vector<Podcast*> _subscriptions;
};

#endif // SUBSCRIPTIONMANAGER_H
