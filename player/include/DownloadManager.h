#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QStandardPaths>
#include <list>

#include "ItemDownload.h"
#include "PodcastItem.h"

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QNetworkAccessManager* networkManager, QObject *parent = nullptr);

    ItemDownload* startDownload(PodcastItem* item);
    void cancelDownload(ItemDownload* download);

    QString downloadLocation() const;
signals:

private slots:
    void finishedDownload(QNetworkReply* reply);

private:
    std::list<ItemDownload> _activeDownloads;
    QNetworkAccessManager* _networkManager;
};

#endif // DOWNLOADMANAGER_H
