#ifndef SUBSCRIPTIONMANAGER_H
#define SUBSCRIPTIONMANAGER_H

#include <QDebug>
#include <QString>
#include <QObject>

#include "feedpp.h"
#include "parser.h"
#include "Podcast.h"

class SubscriptionManager : public QObject
{
    Q_OBJECT

public:
    SubscriptionManager();
    virtual ~SubscriptionManager();

    bool subscribe_to(QString const& url);

    std::vector<Podcast*> const& subscriptions() const { return _subscriptions; }

signals:
    void new_subscription_added();

private:
    feedpp::parser _parser;
    std::vector<Podcast*> _subscriptions;
};

#endif // SUBSCRIPTIONMANAGER_H
