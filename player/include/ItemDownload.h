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
    ItemDownload(PodcastItem* podcast, QNetworkReply* download, QString destination, QObject *parent = nullptr);

    PodcastItem *item() const;
    QNetworkReply *download() const;

    void setItem(PodcastItem *item);
    void setDownload(QNetworkReply *download);

signals:
    //void downloadProgress(qint64, qint64);
    void downloadError(QNetworkReply::NetworkError);
    void downloadFinished(PodcastItem*);

private slots:
    void finished();
    void dataAvailable();
    void downloadProgress(quint64 downloaded, qint64 total);

private:
    PodcastItem* _item;
    QNetworkReply* _download;
    QFile _downloadFile;
};

#endif // ITEMDOWNLOAD_H
