#ifndef EPISODEMODEL_H
#define EPISODEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFont>

#include "Podcast.h"
#include "PodcastItem.h"
#include "Repository.h"
#include "ItemDownload.h"

class EpisodeModel : public QAbstractTableModel {
    Q_OBJECT
public:
    EpisodeModel(Podcast* podcast, Repository& repo, QObject* parent = nullptr);
    virtual ~EpisodeModel() override;

    PodcastItem* episodeFor(QModelIndex const& index);
    void downloadStartedFor(QModelIndex const& index, ItemDownload* download);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void markUnheard(QModelIndex const& index);
    void markListenedTo(QModelIndex const& index);

private slots:
    void downloadFinished(PodcastItem* item);

private:
    Podcast* _source;
    Repository& _repo;
};


#endif // EPISODEMODEL_H
