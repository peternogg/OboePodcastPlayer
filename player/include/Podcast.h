#ifndef PODCAST_H
#define PODCAST_H

#include <QObject>
#include <QDateTime>

#include <algorithm>

#include "feedpp.h"
#include "PodcastItem.h"

class Podcast : public QObject
{
    Q_GADGET
public:
    Podcast(feedpp::feed const& feed, QObject* parent = nullptr);
    virtual ~Podcast();

    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl link READ link)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QDateTime lastUpdate READ lastUpdate)
    Q_PROPERTY(std::vector<PodcastItem*> items READ items)

    QString title() const;
    QUrl link() const;
    QString description() const;
    QDateTime lastUpdate() const;
    std::vector<PodcastItem*> items() const;

private:
    QString _title;
    QUrl _link;
    QString _description;
    QDateTime _lastUpdate;

    std::vector<PodcastItem*> _items;
};

#endif // PODCAST_H
