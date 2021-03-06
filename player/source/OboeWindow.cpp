#include "OboeWindow.h"
#include "ui_OboeWindow.h"

#include "api_key.h"

using namespace std::chrono;

OboeWindow::OboeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OboeWindow),
    _repo(),
    _networkManager(this),
    _downloadManager(&_networkManager, this),
    _subscriptionManager(_repo, _downloadManager, this),
    _queue(),
    _settingsManager(&_repo),
    _searchInterface(SEARCH_API_KEY, &_networkManager),
    _currentEpisodeModel(nullptr),
    _episodeContextMenu(this),
    _podcastContextMenu(this),
    _queueContextMenu(this),
    _lastSelectedPosition(),
    _updateTimer(new QTimer()),
    _settingsWindow(&_settingsManager, this),
    _subscriptionUpdateTime(10 * 60 * 1000),
    _jumpBackwardTime(15 * 1000),
    _jumpForwardTime(15 * 1000)
{
    _subscriptionManager.loadSubscriptions();
    _settingsManager.loadSettings();
    _settingsWindow.loadSettings();

    _subscriptionUpdateTime = _settingsManager.subscriptionUpdatePeriod() * 60 * 1000;
    _jumpBackwardTime = _settingsManager.jumpBackwardAmount();
    _jumpForwardTime = _settingsManager.jumpForwardAmount();

    _networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    ui->setupUi(this);

    ui->subscriptionsList->setModel(&_subscriptionManager);
    ui->subscriptionsList->horizontalHeader()->setStretchLastSection(true);
    ui->subscriptionsList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ui->episodeView->horizontalHeader()->setStretchLastSection(true);
    ui->episodeView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->episodeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->episodeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->queueList->setModel(&_queue);
    ui->queueList->horizontalHeader()->setStretchLastSection(true);
    ui->queueList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ui->searchResults->setModel(&_searchInterface);
    ui->searchResults->horizontalHeader()->setStretchLastSection(true);
    ui->searchResults->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    _episodeContextMenu.addAction(ui->appendToQueue);
    _episodeContextMenu.addAction(ui->prependToQueue);
    _episodeContextMenu.addSeparator();
    _episodeContextMenu.addAction(ui->downloadEpisode);
    _episodeContextMenu.addAction(ui->deleteDownloadedEpisode);
    _episodeContextMenu.addSeparator();
    _episodeContextMenu.addAction(ui->markEpisodeUnheard);
    _episodeContextMenu.addAction(ui->markEpisodeListenedTo);

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

    // Auto updates
    _updateTimer->setInterval(_subscriptionUpdateTime);
    _updateTimer->start();
    connect(_updateTimer, &QTimer::timeout, &_subscriptionManager, &SubscriptionManager::checkForUpdates);

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
    connect(ui->actionUpdate_subscriptions, &QAction::triggered, &_subscriptionManager, &SubscriptionManager::checkForUpdates);
    connect(ui->subscriptionsList, &QTableView::doubleClicked, this, &OboeWindow::showPodcastEpisodes);
    connect(ui->episodeView, &QTableView::doubleClicked, this, &OboeWindow::playEpisode);
    connect(ui->episodeView, &QTableView::customContextMenuRequested, this, &OboeWindow::showEpisodeContextMenu);

    connect(ui->togglePausePlay, &QAction::triggered, &_queue, &PlaybackQueue::togglePlayback);
    connect(ui->togglePausePlay, &QAction::triggered, [this]() {
        ui->togglePausePlay->setIcon(QIcon::fromTheme(_queue.isPlaying() ? ":/icons/pause.png" : ":/icons/play.png"));
    });

    connect(ui->jumpForwards, &QAction::triggered, [this]() {
         _queue.addTime(_jumpForwardTime);
    });

    connect(ui->jumpBackwards, &QAction::triggered, [this]() {
        _queue.addTime(-_jumpBackwardTime);
    });

    connect(ui->deleteDownloadedEpisode, &QAction::triggered, [this]() {
        if (_currentEpisodeModel == nullptr)
            return;

        auto const selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
        auto* const episode = _currentEpisodeModel->episodeFor(selectedItem);

        if (episode == _queue.currentEpisode())
            QMessageBox::warning(this, "Failed to delete", "Can't delete an episode which is playing. Please finish playing the episode, then delete it.");
        else
            _currentEpisodeModel->deleteDownloadedEpisode(selectedItem);
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

    connect(ui->markEpisodeUnheard, &QAction::triggered, [this]() {
        if (_currentEpisodeModel == nullptr)
            return;

        auto const selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
        _currentEpisodeModel->markUnheard(selectedItem);
    });

    connect(ui->markEpisodeListenedTo, &QAction::triggered, [this]() {
        if (_currentEpisodeModel == nullptr)
            return;

        auto const selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
        _currentEpisodeModel->markListenedTo(selectedItem);
    });

    connect(ui->searchEntry, &QLineEdit::returnPressed, ui->performSearch, &QAction::trigger);

    connect(ui->performSearch, &QAction::triggered, [this]() {
        if (ui->searchEntry->text().isEmpty())
            return;

        _searchInterface.beginSearch(ui->searchEntry->text());
    });

    connect(ui->searchResults, &QTableView::doubleClicked, this, &OboeWindow::addNewSubscriptionFromSearch);

    connect(ui->playNext, &QAction::triggered, &_queue, &PlaybackQueue::playNext);
    connect(&_queue, &PlaybackQueue::episodeChanged, this, &OboeWindow::playbackEpisodeChanged);
    connect(&_queue, &PlaybackQueue::positionChanged, this, &OboeWindow::playbackPositionChanged);
    connect(&_queue, &PlaybackQueue::durationChanged, this, &OboeWindow::playbackDurationChanged);
    connect(ui->timeline, &QSlider::sliderMoved, &_queue, &PlaybackQueue::setPosition);

    connect(ui->showSettings, &QAction::triggered, [this]() {
        _settingsWindow.show();
    });

    connect(&_settingsManager, &SettingsManager::settingChanged, [this](const QString& name, const QVariant& value) {
        if (name == "jumpForwardAmount") {
            _jumpForwardTime = value.toInt();
        } else if (name == "jumpBackwardAmount") {
            _jumpBackwardTime = value.toInt();
        } else if (name == "subscriptionUpdatePeriod") {
            _updateTimer->stop();
            _updateTimer->setInterval(minutes(value.toInt()));
            _updateTimer->start();
        }
    });
}

OboeWindow::~OboeWindow()
{
    delete ui;
}

void OboeWindow::closeEvent(QCloseEvent*) {
    // Store all the podcast episodes to disk
    _subscriptionManager.storeSubscriptions();
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
    _subscriptionManager.subscribeTo(string);
}

void OboeWindow::addNewSubscriptionFromSearch(const QModelIndex &index) {
    auto selectedResult = _searchInterface.results()[index.row()];
    _subscriptionManager.subscribeTo(selectedResult.getUrl());
}

void OboeWindow::showPodcastEpisodes(const QModelIndex &index) {
    _currentEpisodeModel = dynamic_cast<EpisodeModel*>(_subscriptionManager.episodesFor(index));
    ui->episodeView->setModel(_currentEpisodeModel);

    ui->goToEpisodeView->trigger();
}

void OboeWindow::showEpisodeContextMenu(QPoint const& pos) {
    _lastSelectedPosition = pos;
    _episodeContextMenu.exec(ui->episodeView->viewport()->mapToGlobal(pos));
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
        ui->timeline->setValue(0);
        ui->timeline->setMaximum(1); // Nothing values
    } else {
        ui->currentEpisodePosition->setText(msToTimestamp(episode->lastTimestamp()));
    }
}

void OboeWindow::playbackDurationChanged(qint64 duration) {
    ui->currentEpisodeLength->setText(msToTimestamp(duration));
    ui->timeline->setMaximum(static_cast<int>(duration));
}
