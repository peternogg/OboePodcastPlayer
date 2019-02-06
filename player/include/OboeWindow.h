#ifndef OBOEWINDOW_H
#define OBOEWINDOW_H

#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QMetaProperty>

#include "SubscriptionManager.h"
#include "SubscriptionListView.h"

#include "Podcast.h"

namespace Ui {
class OboeWindow;
}

class OboeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OboeWindow(QWidget *parent = nullptr);
    ~OboeWindow();

private:
    Ui::OboeWindow *ui;
    SubscriptionManager* _manager;
    SubscriptionListView* _managerView;

public slots:
    void add_new_subscripion_by_url();
};

#endif // OBOEWINDOW_H
