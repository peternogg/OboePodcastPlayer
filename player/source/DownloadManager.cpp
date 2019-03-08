#include "DownloadManager.h"

DownloadManager::DownloadManager(QNetworkAccessManager* networkManager, QObject *parent)
    : QObject(parent), _networkManager(networkManager)
{
    connect(_networkManager, &QNetworkAccessManager::finished,
            this, &DownloadManager::finishedDownload);

    _networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

ItemDownload* DownloadManager::startDownload(PodcastItem* item) {
    QNetworkRequest request(item->enclosureUrl());
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:65.0) Gecko/20100101 Firefox/65.0");

    auto* const reply = _networkManager->get(request);

    item->setDownloadState(DownloadState::Downloading);
    item->setDownloadPath(downloadLocation() + "/" + item->enclosureUrl().fileName());

    _activeDownloads.emplace_back(item, reply, this);

    return &_activeDownloads.back();
}

QString DownloadManager::downloadLocation() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

void DownloadManager::finishedDownload(QNetworkReply* reply) {
    reply->deleteLater();
    qDebug() << "Finished a download.";

    //_activeDownloads.remove_if([reply](ItemDownload& item) { return item.download() == reply; });
}
