#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(SettingsManager* settingsManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow),
    _settingsManager(settingsManager)
{
    ui->setupUi(this);

    connect(ui->subscriptionUpdateRate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsWindow::subscriptionRefreshTimeChanged);
    connect(ui->jumpForwardsTime, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsWindow::jumpForwardsTimeChanged);
    connect(ui->jumpBackwardsTime, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsWindow::jumpBackwardsTimeChanged);
    connect(ui->automaticDownloadCheckBox, &QCheckBox::stateChanged, this, &SettingsWindow::automaticDownloadStateChanged);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::loadSettings() {
    ui->jumpForwardsTime->setValue(_settingsManager->jumpForwardAmount() / 1000);
    ui->jumpBackwardsTime->setValue(_settingsManager->jumpBackwardAmount() / 1000);
    ui->automaticDownloadCheckBox->setChecked(_settingsManager->automaticDownload());

}

void SettingsWindow::subscriptionRefreshTimeChanged(int newIndex) {
    // Times in minutes between refreshes
    const int TIMES[] = { 10, 15, 30, 1 * 60, 2 * 60, 4 * 60, 8 * 60, 24 * 60, -1};
    _settingsManager->setSetting("subscriptionUpdatePeriod", TIMES[newIndex]);
}

void SettingsWindow::jumpForwardsTimeChanged(int newValue) {
    _settingsManager->setSetting("jumpForwardAmount", newValue * 1000);
}

void SettingsWindow::jumpBackwardsTimeChanged(int newValue) {
    _settingsManager->setSetting("jumpBackwardAmount", newValue * 1000);
}

void SettingsWindow::automaticDownloadStateChanged(int newState) {
    _settingsManager->setSetting("automaticDownload", newState == Qt::Checked);
}


