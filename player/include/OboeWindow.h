#ifndef OBOEWINDOW_H
#define OBOEWINDOW_H

#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QMetaProperty>

#include "SubscriptionManager.h"
#include "Repository.h"

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
    Repository _repo;
    SubscriptionManager* _manager;
    QMenu* _menu;

public slots:
    void add_new_subscripion_by_url();
private slots:
    void showPodcastEpisodes(const QModelIndex &index);
    void showEpisodeContextMenu(QPoint const& pos);
};

#endif // OBOEWINDOW_H
