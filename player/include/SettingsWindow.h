#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "SettingsManager.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(SettingsManager* settingsManager, QWidget *parent = nullptr);
    ~SettingsWindow();

    void loadSettings();

private slots:
    void subscriptionRefreshTimeChanged(int newIndex);
    void jumpForwardsTimeChanged(int newValue);
    void jumpBackwardsTimeChanged(int newValue);
    void automaticDownloadStateChanged(int state);

private:
    Ui::SettingsWindow *ui;
    SettingsManager* _settingsManager;
};

#endif // SETTINGSWINDOW_H
