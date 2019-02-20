#include "OboeWindow.h"
#include "ui_OboeWindow.h"

OboeWindow::OboeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OboeWindow),
    _repo{},
    _manager{new SubscriptionManager{_repo}}
{
    _manager->loadSubscriptions();

    ui->setupUi(this);

    ui->subscriptionsList->setModel(_manager);
    ui->subscriptionsList->horizontalHeader()->setStretchLastSection(true);
    ui->subscriptionsList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    // Set up naviation actions
    ui->navSubscriptions->setDefaultAction(ui->goToSubscriptions);
    ui->navSearch->setDefaultAction(ui->goToSearch);
    ui->navPlaylists->setDefaultAction(ui->goToPlaylists);
    ui->navNowPlaying->setDefaultAction(ui->goToNowPlaying);

    connect(ui->goToSubscriptions, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->goToSearch, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->goToPlaylists, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(3);
    });

    connect(ui->goToNowPlaying, &QAction::triggered, [this]() {
       this->ui->stackedWidget->setCurrentIndex(4);
    });

    connect(ui->actionAdd_URL, &QAction::triggered, this, &OboeWindow::add_new_subscripion_by_url);
    connect(ui->actionUpdate_subscriptions, &QAction::triggered, this, &OboeWindow::startUpdatingSubscriptions);
    connect(ui->actionUpdate_subscriptions, &QAction::triggered, _manager, &SubscriptionManager::checkForUpdates);
    connect(_manager, &SubscriptionManager::finishedUpdateSubscriptions, this, &OboeWindow::finishedUpdatingSubscriptions);

    //connect(_manager, &SubscriptionManager::new_subscription_added, this, &OboeWindow::on_new_subscription);
}

OboeWindow::~OboeWindow()
{
    delete ui;
}

void OboeWindow::add_new_subscripion_by_url() {
    auto string = QInputDialog::getText(this, "Podcast RSS URL", "Please enter the URL of a podcast's RSS feed");
    _manager->subscribeTo(string);
}

void OboeWindow::startUpdatingSubscriptions() {
    statusBar()->showMessage("Checking for updates...");
}

void OboeWindow::finishedUpdatingSubscriptions() {
    statusBar()->showMessage("Finished updating.", 60 * 1000);
}

//void OboeWindow::on_new_subscription() {

//}
