#include "ItemDownload.h"

ItemDownload::ItemDownload(PodcastItem* podcast, QNetworkReply* download, QObject *parent)
    : QObject(parent), _item(podcast), _download(download), _downloadFile(podcast->downloadPath())
{
    qDebug() << podcast->downloadPath();
    if (!_downloadFile.open(QFile::WriteOnly)) {
        throw "big error";
    }

    connect(download, &QNetworkReply::downloadProgress, this, &ItemDownload::downloadProgress);
    //connect(download, SIGNAL(download->error), this, SIGNAL(this->downloadError));
    connect(download, &QNetworkReply::finished, this, &ItemDownload::finished);
    //connect(download, &QNetworkReply::readyRead, this, &ItemDownload::dataAvailable);
}

void ItemDownload::finished() {
    qDebug() << "finished downloading " << _item->title();
    _downloadFile.write(_download->readAll());
    _downloadFile.close();

    _item->setDownloadState(PodcastItem::DownloadState::Downloaded);

    emit downloadFinished(_item);
}

void ItemDownload::dataAvailable() {

}

void ItemDownload::downloadProgress(quint64 downloaded, qint64 total) {
    qDebug() << QString("%1 out of %2 finished").arg(downloaded).arg(total);
}

QNetworkReply *ItemDownload::download() const
{
    return _download;
}

void ItemDownload::setDownload(QNetworkReply *download)
{
    _download = download;
}

PodcastItem *ItemDownload::item() const
{
    return _item;
}

void ItemDownload::setItem(PodcastItem *item)
{
    _item = item;
}
