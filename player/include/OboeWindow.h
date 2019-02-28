#ifndef OBOEWINDOW_H
#define OBOEWINDOW_H

#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QMetaProperty>
#include <QStatusBar>
#include <QMenu>

#include "SubscriptionManager.h"
#include "EpisodeModel.h"
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
    QNetworkAccessManager _networkManager;
    DownloadManager _downloadManager;
    SubscriptionManager _manager;

    QMenu _menu;
    QPoint _lastSelectedPosition;

private slots:
    void episodeDownloadRequested();
    void addNewSubscriptionByUrl();
    void showPodcastEpisodes(const QModelIndex &index);
    void showEpisodeContextMenu(QPoint const& pos);
    void downloadFinished(PodcastItem* download);
};

#endif // OBOEWINDOW_H
