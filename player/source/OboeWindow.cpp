#include "OboeWindow.h"
#include "ui_OboeWindow.h"

OboeWindow::OboeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OboeWindow),
    _manager{new SubscriptionManager{}}
{
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
    //connect(_manager, &SubscriptionManager::new_subscription_added, this, &OboeWindow::on_new_subscription);
}

OboeWindow::~OboeWindow()
{
    delete ui;
}

void OboeWindow::add_new_subscripion_by_url() {
    auto string = QInputDialog::getText(this, "Podcast RSS URL", "Please enter the URL of a podcast's RSS feed");
    _manager->subscribe_to(string);
}

//void OboeWindow::on_new_subscription() {

//}
