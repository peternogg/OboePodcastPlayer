#include "ItemDownload.h"

ItemDownload::ItemDownload(PodcastItem* podcast, QNetworkReply* download, QObject *parent)
    : QObject(parent), _podcast(podcast), _download(download), _downloadFile(podcast->downloadPath())
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
    emit downloadFinished();
    qDebug() << "finished downloading " << _podcast->title();
    _downloadFile.write(_download->readAll());
    _downloadFile.close();
}

void ItemDownload::dataAvailable() {

}

void ItemDownload::downloadProgress(quint64 downloaded, qint64 total) {
    qDebug() << QString("%1 out of %2 finished").arg(downloaded).arg(total);
}
