#ifndef OBOEWINDOW_H
#define OBOEWINDOW_H

#include <chrono>

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QMetaProperty>
#include <QStatusBar>
#include <QTimer>
#include <QMenu>

#include "SubscriptionManager.h"
#include "EpisodeModel.h"
#include "Repository.h"
#include "PlaybackQueue.h"
#include "SettingsManager.h"
#include "util.h"

namespace Ui {
class OboeWindow;
}

class OboeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OboeWindow(QWidget *parent = nullptr);
    virtual ~OboeWindow() override;

private:
    Ui::OboeWindow *ui;

    Repository _repo;
    QNetworkAccessManager _networkManager;
    DownloadManager _downloadManager;
    SubscriptionManager _subscriptionManager;
    PlaybackQueue _queue;
    SettingsManager _settingsManager;

    EpisodeModel* _currentEpisodeModel;

    QMenu _episodeContextMenu;
    QMenu _podcastContextMenu;
    QMenu _queueContextMenu;

    QPoint _lastSelectedPosition;
    QTimer* _updateTimer;

    // Settings
    int _subscriptionUpdateTime;
    qint64 _jumpBackwardTime;
    qint64 _jumpForwardTime;

protected:
    virtual void closeEvent(QCloseEvent*) override;

private slots:
    void episodeDownloadRequested();
    void addNewSubscriptionByUrl();
    void showPodcastEpisodes(const QModelIndex &index);
    void showEpisodeContextMenu(QPoint const& pos);
    void downloadFinished(PodcastItem* download);
    void playEpisode(const QModelIndex& index);
    void playbackPositionChanged(qint64 position);
    void playbackEpisodeChanged(PodcastItem const* episode);
    void playbackDurationChanged(qint64 duration);
};

#endif // OBOEWINDOW_H
