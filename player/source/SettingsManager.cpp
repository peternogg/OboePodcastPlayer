#include "SettingsManager.h"

char const* const SETTINGS[] = { "subscriptionUpdatePeriod", "jumpForwardAmount", "jumpBackwardAmount", "automaticDownload" };

SettingsManager::SettingsManager(Repository* repo, QObject *parent)
    : QObject(parent), _repo(repo), _settings()
{}

void SettingsManager::loadSettings() {
    for (auto setting : SETTINGS) {
        QVariant value = _repo->fetchSetting(setting);
        if (value.isValid())
            setSetting(setting, value);
        else
            qDebug() << "Failed to fetch " << setting;
    }
}

void SettingsManager::setSetting(QString const& name, QVariant const& value) {
    if (std::find(std::begin(SETTINGS), std::end(SETTINGS), name) == std::end(SETTINGS)) {
        qDebug() << "Attempt to set unknown setting " << name << " to " << value;
        return;
    }

    _settings.insert_or_assign(name, value);
    _repo->storeSetting(name, value);
    emit settingChanged(name, value);
}

QVariant SettingsManager::fetchSetting(QString const& name) const {
    if (auto iter = _settings.find(name); iter != _settings.end()) {
        return iter->second;
    }

    return {};
}
