#include "SubscriptionManager.h"

SubscriptionManager::SubscriptionManager()
    : QObject{}, _parser{}, _subscriptions{}
{}

SubscriptionManager::~SubscriptionManager() {}

bool SubscriptionManager::subscribe_to(QString const& url) {
    auto feed = _parser.parse_url(url.toStdString());
    qDebug() << QString::fromStdString(feed.title) << ": " << QString::fromStdString(feed.pubDate);

    _subscriptions.push_back(new Podcast(feed));

    emit new_subscription_added();

    return true;
}

