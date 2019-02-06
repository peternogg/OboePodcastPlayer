#include "SubscriptionListView.h"

SubscriptionListView::SubscriptionListView(SubscriptionManager const* manager)
    : QAbstractListModel{}, _manager{manager}
{
    connect(_manager, &SubscriptionManager::new_subscription_added, this, &SubscriptionListView::new_subscription_added);
}

SubscriptionListView::~SubscriptionListView() {}

int SubscriptionListView::rowCount(QModelIndex const& parent) const {
    if (parent.isValid())
        return 0;

    return static_cast<int>(_manager->subscriptions().size());
}

QVariant SubscriptionListView::data(QModelIndex const& index, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant::Invalid;

    return _manager->subscriptions()[static_cast<size_t>(index.row())]->title();
}

void SubscriptionListView::new_subscription_added() {
    emit dataChanged(QModelIndex(), QModelIndex());
}
