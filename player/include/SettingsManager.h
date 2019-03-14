#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <unordered_map>
#include <functional>

#include <QObject>

#include "Repository.h"

namespace std {
template<> struct hash<QString> {
    std::size_t operator()(QString const& s) const {
        return qHash(s);
    }
};
}

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    explicit SettingsManager(Repository* repo, QObject *parent = nullptr);
    QVariant fetchSetting(QString const& name) const;

    int subscriptionUpdatePeriod() const { return fetchSetting("subscriptionUpdatePeriod").toInt(); }
    qint64 jumpForwardAmount() const { return fetchSetting("jumpForwardAmount").toLongLong(); }
    qint64 jumpBackwardAmount() const { return fetchSetting("jumpBackwardAmount").toLongLong(); }
    bool automaticDownload() const { return fetchSetting("automaticDownload").toBool(); }

signals:
    void settingChanged(QString const& name, QVariant const& value);

public slots:
    void loadSettings();
    void setSetting(QString const& name, QVariant const& value);

private:
    Repository* _repo;
    std::unordered_map<QString, QVariant> _settings;
};

#endif // SETTINGSMANAGER_H
