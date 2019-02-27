#ifndef ITEMDOWNLOAD_H
#define ITEMDOWNLOAD_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>

#include "PodcastItem.h"

class ItemDownload : public QObject
{
    Q_OBJECT
public:
    ItemDownload(PodcastItem* podcast, QNetworkReply* download, QObject *parent = nullptr);

signals:
    //void downloadProgress(qint64, qint64);
    void downloadError(QNetworkReply::NetworkError);
    void downloadFinished();

private slots:
    void finished();
    void dataAvailable();
    void downloadProgress(quint64 downloaded, qint64 total);

private:
    PodcastItem* _podcast;
    QNetworkReply* _download;
    QFile _downloadFile;
};

#endif // ITEMDOWNLOAD_H
