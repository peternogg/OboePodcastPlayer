#include "OboeWindow.h"
#include "ui_OboeWindow.h"

OboeWindow::OboeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OboeWindow),
    _repo(),
    _networkManager(this),
    _downloadManager(&_networkManager, this),
    _manager(_repo, _downloadManager, this),
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

    _menu.addAction(ui->downloadEpisode);
    _menu.addAction(ui->deleteDownloadedEpisode);

    // Set up naviation actions
    ui->navSubscriptions->setDefaultAction(ui->goToSubscriptions);
    ui->navSearch->setDefaultAction(ui->goToSearch);
    ui->navSummary->setDefaultAction(ui->goToSummary);
    ui->navNowPlaying->setDefaultAction(ui->goToNowPlaying);

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
    //connect(_manager, &SubscriptionManager::new_subscription_added, this, &OboeWindow::on_new_subscription);
}

OboeWindow::~OboeWindow()
{
    delete ui;
}

void OboeWindow::episodeDownloadRequested() {
    QModelIndex selectedItem = ui->episodeView->indexAt(_lastSelectedPosition);
    auto* const model = static_cast<EpisodeModel*>(ui->episodeView->model());
    auto* const episode = model->episodeFor(selectedItem);

    if (episode == nullptr) {
        statusBar()->showMessage("Error: Failed to download podcast.");
        return;
    }

    auto* downloadRecord = _downloadManager.startDownload(episode);
    model->downloadStartedFor(selectedItem, downloadRecord);
}

void OboeWindow::addNewSubscriptionByUrl() {
    auto string = QInputDialog::getText(this, "Podcast RSS URL", "Please enter the URL of a podcast's RSS feed");
    _manager.subscribeTo(string);
}

void OboeWindow::showPodcastEpisodes(const QModelIndex &index)
{
    auto* model = _manager.episodesFor(index);
    ui->episodeView->setModel(model);

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
    auto* const model = static_cast<EpisodeModel*>(ui->episodeView->model());
    auto* podcast = model->episodeFor(index);

    _queue.addEpisode(podcast);
    _queue.playNext();
}
