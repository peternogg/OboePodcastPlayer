#ifndef SUBSCRIPTIONLISTMODEL_H
#define SUBSCRIPTIONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "SubscriptionManager.h"

class SubscriptionListView : public QAbstractListModel
{
    Q_OBJECT
public:
    SubscriptionListView(SubscriptionManager const* manager);
    virtual ~SubscriptionListView() override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

private slots:
    void new_subscription_added();

private:
    SubscriptionManager const* _manager;
};

#endif // SUBSCRIPTIONLISTMODEL_H
