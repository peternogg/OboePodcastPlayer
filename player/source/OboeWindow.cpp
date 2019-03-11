#include "OboeWindow.h"
#include "ui_OboeWindow.h"

OboeWindow::OboeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OboeWindow),
    _repo(),
    _networkManager(this),
    _downloadManager(&_networkManager, this),
    _manager(_repo, _downloadManager, this),
    _queue(),
    _currentEpisodeModel(nullptr),
    _menu(this),
    _lastSelectedPosition()
{
    _manager.loadSubscriptions();

    ui->setupUi(this);

    ui->subscriptionsList->setModel(&_manager);
    ui->subscriptionsList->horizontalHeader()->setStretchLastSection(true);
    ui->subscriptionsList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ui->episodeView->horizontalHeader()->setStretchLastSection(true);
    ui->episodeView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    //ui->episodeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->episodeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->episodeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->queueList->setModel(&_queue);
    ui->queueList->horizontalHeader()->setStretchLastSection(true);
    ui->queueList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    _menu.addAction(ui->appendToQueue);
    _menu.addAction(ui->prependToQueue);
    _menu.addAction(ui->downloadEpisode);
    _menu.addAction(ui->deleteDownloadedEpisode);

    // Set up naviation actions
    ui->navSubscriptions->setDefaultAction(ui->goToSubscriptions);
    ui->navSearch->setDefaultAction(ui->goToSearch);
    ui->navSummary->setDefaultAction(ui->goToSummary);
    ui->navNowPlaying->setDefaultAction(ui->goToNowPlaying);

    ui->pausePlay->setDefaultAction(ui->togglePausePlay);
    ui->jumpBack->setDefaultAction(ui->jumpBackwards);
    ui->jumpForward->setDefaultAction(ui->jumpForwards);
    ui->skipCurrent->setDefaultAction(ui->playNext);
    ui->searchStart->setDefaultAction(ui->performSearch);

    connect(ui->goToSubscriptions, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->goToSearch, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->goToSummary, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(3);
    });

    connect(ui->goToNowPlaying, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(4);
    });

    connect(ui->goToEpisodeView, &QAction::triggered, [this]() {
        this->ui->stackedWidget->setCurrentIndex(5);
    });

    connect(ui->downloadEpisode, &QAction::triggered, this, &OboeWindow::episodeDownloadRequested);
    connect(ui->actionAdd_URL, &QAction::triggered, this, &OboeWindow::addNewSubscriptionByUrl);
    connect(ui->actionUpdate_subscriptions, &QAction::triggered, &_manager, &SubscriptionManager::checkForUpdates);
    connect(ui->subscriptionsList, &QTableView::doubleClicked, this, &OboeWindow::showPodcastEpisodes);
    connect(ui->episodeView, &QTableView::doubleClicked, this, &OboeWindow::playEpisode);
    connect(ui->episodeView, &QTableView::customContextMenuRequested, this, &OboeWindow::showEpisodeContextMenu);

    connect(ui->togglePausePlay, &QAction::triggered, &_queue, &PlaybackQueue::togglePlayback);
    connect(ui->togglePausePlay, &QAction::triggered, [this]() {
        ui->togglePausePlay->setIcon(QIcon::fromTheme(_queue.isPlaying() ? ":/icons/pause.png" : ":/icons/play.png"));
    });

    connect(ui->appendToQueue, &QAction::triggered, [this]() {
        if (_currentEpisodeModel == nullptr)
            return;

        auto const selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
        auto* const episode = _currentEpisodeModel->episodeFor(selectedItem);

        _queue.appendEpisode(episode);
    });

    connect(ui->prependToQueue, &QAction::triggered, [this]() {
        if (_currentEpisodeModel == nullptr)
            return;

        auto const selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
        auto* const episode = _currentEpisodeModel->episodeFor(selectedItem);

        _queue.prependEpisode(episode);
    });

    connect(&_queue, &PlaybackQueue::episodeChanged, this, &OboeWindow::playbackEpisodeChanged);
    connect(&_queue, &PlaybackQueue::positionChanged, this, &OboeWindow::playbackPositionChanged);
    connect(&_queue, &PlaybackQueue::durationChanged, this, &OboeWindow::playbackDurationChanged);
    connect(ui->timeline, &QSlider::sliderMoved, &_queue, &PlaybackQueue::setPosition);
    //connect(_manager, &SubscriptionManager::new_subscription_added, this, &OboeWindow::on_new_subscription);
}

OboeWindow::~OboeWindow()
{
    delete ui;
}

void OboeWindow::episodeDownloadRequested() {
    QModelIndex selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
    auto* const episode = _currentEpisodeModel->episodeFor(selectedItem);

    if (episode == nullptr) {
        statusBar()->showMessage("Error: Failed to download podcast.");
        return;
    }

    auto* downloadRecord = _downloadManager.startDownload(episode);
    _currentEpisodeModel->downloadStartedFor(selectedItem, downloadRecord);
}

void OboeWindow::addNewSubscriptionByUrl() {
    auto string = QInputDialog::getText(this, "Podcast RSS URL", "Please enter the URL of a podcast's RSS feed");
    _manager.subscribeTo(string);
}

void OboeWindow::showPodcastEpisodes(const QModelIndex &index) {
    _currentEpisodeModel = dynamic_cast<EpisodeModel*>(_manager.episodesFor(index));
    ui->episodeView->setModel(_currentEpisodeModel);

    ui->goToEpisodeView->trigger();
}

void OboeWindow::showEpisodeContextMenu(QPoint const& pos) {
    _lastSelectedPosition = pos;
    _menu.exec(ui->episodeView->viewport()->mapToGlobal(pos));
}

void OboeWindow::downloadFinished(PodcastItem* item) {
    _repo.store(item);

}

void OboeWindow::playEpisode(const QModelIndex& index) {
    if (_currentEpisodeModel == nullptr)
        return;

    auto* podcast = _currentEpisodeModel->episodeFor(index);

    if (podcast->downloadState() != PodcastItem::Downloaded) {
        QMessageBox::warning(this, "Can't play that episode", "You must download that episode before you play it.");
        return;
    }

    _queue.prependEpisode(podcast);
    _queue.playNext();
    ui->togglePausePlay->setIcon(QIcon::fromTheme(":/icons/pause.png"));
}

void OboeWindow::playbackPositionChanged(qint64 position) {
    ui->currentEpisodePosition->setText(msToTimestamp(position));
    ui->timeline->setValue(static_cast<int>(position));
}

void OboeWindow::playbackEpisodeChanged(PodcastItem const* episode) {
    if (episode == nullptr) {
        ui->currentEpisodeLength->setText("-");
        ui->currentEpisodePosition->setText("-");
    } else {
        ui->currentEpisodePosition->setText(msToTimestamp(episode->lastTimestamp()));
    }
}

void OboeWindow::playbackDurationChanged(qint64 duration) {
    ui->currentEpisodeLength->setText(msToTimestamp(duration));
    ui->timeline->setMaximum(static_cast<int>(duration));
}
